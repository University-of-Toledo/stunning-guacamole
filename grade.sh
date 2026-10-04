#!/usr/bin/env bash
#
# Author:      Merl Creps
# Description: Grade every student branch of a repo (all branches except main).
#              For each branch it:
#                1. checks out the branch into its own folder
#                2. runs make clean, make, and make run (fed from input.txt)
#                3. compares the output to expected_output.txt
#                   PASS            exact match
#                   PASS_WS         matches when whitespace/blank lines are ignored
#                   FAIL            neither matches (diff saved for feedback)
#                4. extracts the text of the student's Word doc (.docx) and grades
#                   it against answer_key.txt with the Claude API
#              Results go to results/results.csv plus one folder per branch.
#
# Usage:       ./grade.sh [-b branch] <repo-url-or-path> <lab-folder> <tests-folder>
#
# lab-folder:  the lab to grade in each branch, e.g. lab03. Student work is
#              in lab03/solution - that folder is built, run, and searched for
#              the Word doc. If there is no solution folder, lab03 itself is used.
# Example:     ./grade.sh git@github.com:University-of-Toledo/stunning-guacamole.git lab03 tests/lab03
#              ./grade.sh -b jsmith git@github.com:University-of-Toledo/stunning-guacamole.git lab03 tests/lab03
#
#   -b branch    grade only this branch and print full feedback (build errors,
#                output differences) to the screen. Students can use this to
#                check their work before the deadline.
#   -g           calculate a points grade (see POINTS below) and add total and
#                percent columns to the results
#   -x branch    skip this branch (e.g. your own); repeat -x for more
#
# Word docs: any .docx identical to a file on main (your instructions) is
# ignored, so only the student's own .docx gets graded.
#
# tests-folder must contain:
#   input.txt            stdin fed to the program
#   expected_output.txt  exactly what the program should print
#   answer_key.txt       answers for the Word doc questions (optional)
#
# Environment:
#   ANTHROPIC_API_KEY    required for Word doc grading (skipped if not set)
#   CLAUDE_MODEL         model to use (default below)
#   RUN_TIMEOUT          seconds before a program is killed (default 10)
#
# POINTS (used with -g; override with environment variables):
#   BUILD_PTS    builds with make                      (default 2)
#   RUN_PTS      runs without timing out               (default 2)
#   OUTPUT_PTS   output PASS                           (default 4)
#                output PASS_WS gets half of OUTPUT_PTS
#   Word doc     points come from the answer key (Claude returns score/max)
#
# WARNING: make runs whatever is in a student's Makefile. Run this on a
#          throwaway machine or container (e.g. the Raspberry Pi), not your laptop.

set -u

# ---------------------------------------------------------------- settings
CLAUDE_MODEL="${CLAUDE_MODEL:-claude-sonnet-4-5}"
RUN_TIMEOUT="${RUN_TIMEOUT:-10}"
MAIN_BRANCH="main"
BUILD_PTS="${BUILD_PTS:-2}"
RUN_PTS="${RUN_PTS:-2}"
OUTPUT_PTS="${OUTPUT_PTS:-4}"

# ---------------------------------------------------------------- arguments
usage() {
    echo "Usage: $0 [-b branch] [-g] [-x branch] <repo-url-or-path> <lab-folder> <tests-folder>" >&2
    exit 1
}

ONLY_BRANCH=""
CALC_GRADE=0
EXCLUDE=""
while getopts ":b:gx:" opt; do
    case "$opt" in
        b) ONLY_BRANCH="$OPTARG" ;;
        g) CALC_GRADE=1 ;;
        x) EXCLUDE="$EXCLUDE$OPTARG"$'\n' ;;
        *) usage ;;
    esac
done
shift $((OPTIND - 1))

[ $# -eq 3 ] || usage

REPO="$1"
LAB_DIR="$2"
TESTS_DIR="$(cd "$3" 2>/dev/null && pwd)" || { echo "Error: tests folder '$3' not found." >&2; exit 1; }

INPUT="$TESTS_DIR/input.txt"
EXPECTED="$TESTS_DIR/expected_output.txt"
ANSWER_KEY="$TESTS_DIR/answer_key.txt"

for f in "$INPUT" "$EXPECTED"; do
    if [ ! -f "$f" ]; then
        echo "Error: missing $f" >&2
        exit 1
    fi
done

# ---------------------------------------------------------------- tools
# macOS has gtimeout (brew install coreutils) instead of timeout
if command -v timeout >/dev/null 2>&1; then
    TIMEOUT="timeout"
elif command -v gtimeout >/dev/null 2>&1; then
    TIMEOUT="gtimeout"
else
    echo "Error: need 'timeout' (Linux) or 'gtimeout' (macOS: brew install coreutils)." >&2
    exit 1
fi

for tool in git make diff unzip perl; do
    command -v "$tool" >/dev/null 2>&1 || { echo "Error: '$tool' is not installed." >&2; exit 1; }
done

GRADE_DOCS=1
if [ -z "${ANTHROPIC_API_KEY:-}" ]; then
    echo "Note: ANTHROPIC_API_KEY not set - Word doc grading skipped."
    GRADE_DOCS=0
elif [ ! -f "$ANSWER_KEY" ]; then
    echo "Note: no answer_key.txt in $TESTS_DIR - Word doc grading skipped."
    GRADE_DOCS=0
elif ! command -v curl >/dev/null 2>&1 || ! command -v jq >/dev/null 2>&1; then
    echo "Note: curl and jq are needed for Word doc grading - skipped."
    GRADE_DOCS=0
fi

# ---------------------------------------------------------------- folders
WORK="$(pwd)/work"
RESULTS="$(pwd)/results"
rm -rf "$WORK" "$RESULTS"
mkdir -p "$WORK" "$RESULTS"

CSV="$RESULTS/results.csv"
if [ "$CALC_GRADE" -eq 1 ]; then
    echo "branch,build,run,output,doc_score,doc_max,total,possible,percent,doc_feedback" > "$CSV"
else
    echo "branch,build,run,output,doc_score,doc_max,doc_feedback" > "$CSV"
fi

# ---------------------------------------------------------------- helpers

# Quote a value for CSV: wrap in quotes, double any quotes inside.
csv_quote() {
    printf '"%s"' "$(printf '%s' "$1" | tr '\n' ' ' | sed 's/"/""/g')"
}

# Print the plain text of a .docx (paragraphs become lines).
docx_text() {
    unzip -p "$1" word/document.xml 2>/dev/null |
        perl -pe 's/<\/w:p>/\n/g; s/<w:tab\/>/\t/g; s/<[^>]+>//g;
                  s/&lt;/</g; s/&gt;/>/g; s/&quot;/"/g; s/&apos;/'"'"'/g; s/&amp;/&/g'
}

# Grade answers against the key with Claude.
# Prints: score<TAB>max<TAB>feedback   (or an error message in feedback)
grade_doc() {
    local answers="$1"
    local key prompt payload response text

    key="$(cat "$ANSWER_KEY")"
    prompt="You are grading a student's written answers for a C programming lab.

ANSWER KEY:
$key

STUDENT ANSWERS:
$answers

Grade each answer against the key. Give partial credit where the idea is right but incomplete.
Reply with ONLY a JSON object, no other text:
{\"score\": <points earned>, \"max\": <points possible>, \"feedback\": \"<one or two sentences for the student>\"}"

    payload="$(jq -n --arg model "$CLAUDE_MODEL" --arg prompt "$prompt" \
        '{model: $model, max_tokens: 1024, messages: [{role: "user", content: $prompt}]}')"

    response="$(curl -s https://api.anthropic.com/v1/messages \
        -H "x-api-key: $ANTHROPIC_API_KEY" \
        -H "anthropic-version: 2023-06-01" \
        -H "content-type: application/json" \
        -d "$payload")"

    text="$(printf '%s' "$response" | jq -r '.content[0].text // empty' 2>/dev/null)"
    if [ -z "$text" ]; then
        printf '\t\tAPI error: %s' "$(printf '%s' "$response" | jq -r '.error.message // "no response"' 2>/dev/null)"
        return
    fi

    # Pull the JSON object out even if the model wrapped it in other text.
    printf '%s' "$text" | perl -0ne 'print $1 if /(\{.*\})/s' |
        jq -r '[(.score|tostring), (.max|tostring), .feedback] | @tsv' 2>/dev/null ||
        printf '\t\tCould not parse grade: %s' "$text"
}

# ---------------------------------------------------------------- clone
echo "Cloning $REPO ..."
if ! git clone --quiet "$REPO" "$WORK/repo"; then
    echo "Error: could not clone $REPO" >&2
    exit 1
fi
cd "$WORK/repo" || exit 1
git fetch --all --quiet

BRANCHES="$(git branch -r --format='%(refname:short)' |
    grep -v -- '->' | grep -v "^origin$" | sed 's#^origin/##' | grep -vx "$MAIN_BRANCH" | sort)"

if [ -n "$EXCLUDE" ]; then
    BRANCHES="$(printf '%s\n' "$BRANCHES" | grep -vxF -f <(printf '%s' "$EXCLUDE"))"
fi

# Fingerprints of every file on main, so instructor .docx files copied into
# student branches are not mistaken for the student's answers.
MAIN_BLOBS="$(git ls-tree -r "origin/$MAIN_BRANCH" 2>/dev/null | awk '{print $3}')"

if [ -n "$ONLY_BRANCH" ]; then
    if ! printf '%s\n' "$BRANCHES" | grep -qx -- "$ONLY_BRANCH"; then
        echo "Error: branch '$ONLY_BRANCH' not found." >&2
        exit 1
    fi
    BRANCHES="$ONLY_BRANCH"
fi

if [ -z "$BRANCHES" ]; then
    echo "No branches other than $MAIN_BRANCH found."
    exit 0
fi

# ---------------------------------------------------------------- grade each branch
for branch in $BRANCHES; do
    safe="$(printf '%s' "$branch" | tr '/' '_')"
    dir="$WORK/branches/$safe"
    out="$RESULTS/$safe"
    mkdir -p "$out"

    echo
    echo "=== $branch ==="

    git worktree add --quiet --detach "$dir" "origin/$branch" 2>"$out/checkout.log"
    # Student work is in <lab-folder>/solution; if that's missing, use <lab-folder>
    if [ -d "$dir/$LAB_DIR/solution" ]; then
        lab="$dir/$LAB_DIR/solution"
        where="$LAB_DIR/solution"
    else
        lab="$dir/$LAB_DIR"
        where="$LAB_DIR"
    fi
    [ -d "$lab" ] && echo "  folder: $where"

    build="no"; run="no"; output="FAIL"
    score=""; max=""; feedback=""

    if [ ! -d "$lab" ]; then
        echo "  no $LAB_DIR folder"
        feedback="No $LAB_DIR folder found."
    elif [ ! -f "$lab/Makefile" ] && [ ! -f "$lab/makefile" ]; then
        echo "  no Makefile in $where"
        feedback="No Makefile found in $where."
    else
        # make clean, then make
        ( cd "$lab" && make clean >/dev/null 2>&1; make ) >"$out/build.log" 2>&1
        if [ $? -eq 0 ]; then
            build="yes"
            echo "  build: ok"

            # make run, fed from input.txt (-s keeps make's own command echo out of the output).
            # If the Makefile has no run target, run the program named by TARGET instead.
            if ( cd "$lab" && make -n run >/dev/null 2>&1 ); then
                run_cmd=(make -s run)
            else
                target="$(cd "$lab" && make -s --no-print-directory \
                    --eval='__grade_target: ; @echo $(TARGET)' __grade_target 2>/dev/null)"
                run_cmd=("./$target")
                echo "  note:  no 'run' target, running ./$target"
            fi
            ( cd "$lab" && "$TIMEOUT" "$RUN_TIMEOUT" "${run_cmd[@]}" ) \
                <"$INPUT" >"$out/actual_output.txt" 2>"$out/run_errors.txt"
            rc=$?
            if [ $rc -eq 124 ]; then
                run="timeout"
                echo "  run:   timed out after ${RUN_TIMEOUT}s"
            else
                run="yes"
                echo "  run:   exit code $rc"
            fi

            # strict, then loose
            if diff "$EXPECTED" "$out/actual_output.txt" >"$out/diff.txt"; then
                output="PASS"
                rm -f "$out/diff.txt"
            elif diff -w -B "$EXPECTED" "$out/actual_output.txt" >/dev/null; then
                output="PASS_WS"
            fi
            echo "  output: $output"
        else
            echo "  build: FAILED (see build.log)"
        fi
    fi

    # Word doc
    doc=""
    while IFS= read -r f; do
        if ! printf '%s\n' "$MAIN_BLOBS" | grep -qx "$(git hash-object "$f")"; then
            doc="$f"
            break
        fi
    done < <(find "$lab" -maxdepth 2 -name '*.docx' ! -name '~$*' 2>/dev/null | sort)
    if [ -z "$doc" ]; then
        echo "  doc:   no student .docx found"
        [ -z "$feedback" ] && feedback="No student Word doc found."
    else
        docx_text "$doc" >"$out/answers.txt"
        if [ "$GRADE_DOCS" -eq 1 ]; then
            IFS=$'\t' read -r score max feedback < <(grade_doc "$(cat "$out/answers.txt")")
            echo "  doc:   $score / $max"
        else
            echo "  doc:   found (not graded)"
        fi
    fi

    # Points grade (-g)
    if [ "$CALC_GRADE" -eq 1 ]; then
        total=0
        possible=$((BUILD_PTS + RUN_PTS + OUTPUT_PTS))
        [ "$build" = "yes" ] && total=$((total + BUILD_PTS))
        [ "$run" = "yes" ] && total=$((total + RUN_PTS))
        case "$output" in
            PASS)    total=$((total + OUTPUT_PTS)) ;;
            PASS_WS) total=$((total + OUTPUT_PTS / 2)) ;;
        esac
        # Doc points may be decimals (partial credit), so use awk for the math
        total="$(awk -v t="$total" -v s="${score:-0}" 'BEGIN { printf "%g", t + s }')"
        possible="$(awk -v p="$possible" -v m="${max:-0}" 'BEGIN { printf "%g", p + m }')"
        percent="$(awk -v t="$total" -v p="$possible" 'BEGIN { printf "%.1f", (p > 0 ? 100 * t / p : 0) }')"
        echo "  grade: $total / $possible ($percent%)"

        printf '%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n' \
            "$(csv_quote "$branch")" "$build" "$run" "$output" "$score" "$max" \
            "$total" "$possible" "$percent" "$(csv_quote "$feedback")" >>"$CSV"
    else
        printf '%s,%s,%s,%s,%s,%s,%s\n' \
            "$(csv_quote "$branch")" "$build" "$run" "$output" "$score" "$max" "$(csv_quote "$feedback")" >>"$CSV"
    fi

    # Single-branch mode (-b): show full feedback on screen
    if [ -n "$ONLY_BRANCH" ]; then
        echo
        if [ "$build" = "no" ] && [ -f "$out/build.log" ]; then
            echo "----- build errors -----"
            cat "$out/build.log"
        fi
        if [ "$run" = "timeout" ]; then
            echo "----- program timed out: check for an infinite loop or a missing input check -----"
        fi
        if [ -s "$out/run_errors.txt" ]; then
            echo "----- errors printed while running -----"
            cat "$out/run_errors.txt"
        fi
        if [ "$output" != "PASS" ] && [ -f "$out/diff.txt" ]; then
            echo "----- output differences (< expected   > yours) -----"
            cat "$out/diff.txt"
        fi
        if [ -n "$feedback" ]; then
            echo "----- Word doc feedback -----"
            echo "$feedback"
        fi
    fi
done

# ---------------------------------------------------------------- clean up
cd "$WORK/repo" && git worktree prune

echo
echo "Done. Results: $CSV"
