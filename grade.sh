#!/usr/bin/env bash
#
# Author:      Merl Creps
# Description: Grade a lab using its Makefile.
#
# Students:    ./grade.sh lab03
#                builds and runs lab03/solution with its Makefile
#                (make clean, make, make run), answers each prompt from
#                lab03/input.txt, and compares the output to lab03/expected.txt.
#                The report is saved to graded.txt (headed with the git branch
#                name, or the current folder if not in a git repo).
#
#              ./grade.sh -e lab03/expected_32.txt lab03
#                same, but compares to the expected file you name
#
# Instructor:  ./grade.sh -g -u <gh-user> -t <gh-token> <repo-url> lab03
#                pulls every branch (one branch per student) from GitHub,
#                grades lab03 in each, and writes graded.txt (full report per
#                student: compile errors, program output, runtime errors,
#                differences) and results.csv (one line per student)
#
# Folder layout:
#   lab03/
#     input.txt         prompt=answer, one per line
#     expected.txt      what the program should print
#     solution/
#       Makefile
#       *.c
#
# input.txt example (when the program prints the prompt, the answer is sent):
#   Enter an integer:=5
#   Enter your name:=Merl
# Prompts are answered in the order listed. No input.txt = no input.
# Needs 'expect' (built into macOS; Linux: sudo dnf/apt install expect).
#
# Results:  PASS     output matches (spaces, tabs and blank lines are ignored)
#           FAIL     does not match (differences are shown)

RUN_TIMEOUT=10

command -v expect >/dev/null 2>&1 || { echo "Error: 'expect' is not installed (Linux: sudo dnf install expect)." >&2; exit 1; }

usage() {
    echo "Usage: $0 [-e expected-file] <lab-folder>   grade your lab" >&2
    echo "       $0 -g -u <gh-user> -t <gh-token> <repo-url> <lab-folder>   grade every student branch" >&2
    exit 1
}

# Trim lines, squeeze spaces/tabs to one space, drop blank lines.
normalize() {
    tr '\t' ' ' | sed 's/  */ /g; s/^ //; s/ $//' | grep -v '^$'
}

# Grade one lab folder. Prints the result and sets RESULT.
grade_lab() {
    local labdir="$1"
    local sol="$labdir/solution"
    local expected="${EXPECTED_FILE:-$labdir/expected.txt}"
    local input; input="$(cd "$labdir" && pwd)/input.txt"
    local actual rc

    RESULT="FAIL"

    if [ ! -f "$sol/Makefile" ] && [ ! -f "$sol/makefile" ]; then
        echo "  no Makefile in $sol"
        RESULT="NO_MAKEFILE"
        return
    fi
    if [ ! -f "$expected" ]; then
        echo "  expected file not found: $expected"
        RESULT="NO_EXPECTED"
        return
    fi

    local buildlog; buildlog="$(mktemp)"
    if ! ( cd "$sol" && make clean >/dev/null 2>&1; make ) >"$buildlog" 2>&1; then
        echo "  build: FAILED"
        echo "  ----- compile errors -----"
        cat "$buildlog"
        echo "  --------------------------"
        rm -f "$buildlog"
        RESULT="BUILD_FAILED"
        return
    fi
    rm -f "$buildlog"
    echo "  build: ok"

    # Run with make run. Each prompt in input.txt is answered when it appears.
    local tmp; tmp="$(mktemp)"
    ( cd "$sol" && expect -c '
        set timeout '"$RUN_TIMEOUT"'
        log_user 1
        spawn -noecho make -s run
        if {[file exists "'"$input"'"]} {
            set f [open "'"$input"'" r]
            while {[gets $f line] >= 0} {
                if {[string trim $line] eq "" || [string index [string trim $line] 0] eq "#"} continue
                set i [string first "=" $line]
                if {$i < 0} continue
                set prompt [string trim [string range $line 0 [expr {$i - 1}]]]
                set answer [string trim [string range $line [expr {$i + 1}] end]]
                expect {
                    -exact $prompt { send -- "$answer\r" }
                    timeout { puts "\n\[grade.sh: prompt not found: $prompt\]"; exit 124 }
                    eof { puts "\n\[grade.sh: program ended before prompt: $prompt\]"; exit [lindex [wait] 3] }
                }
            }
            close $f
        }
        expect {
            timeout { exit 124 }
            eof
        }
        exit [lindex [wait] 3]
    ' ) > "$tmp"
    rc=$?
    actual="$(tr -d '\r' < "$tmp")"
    rm -f "$tmp"
    if [ "$rc" -eq 124 ]; then
        echo "  run:   timed out after ${RUN_TIMEOUT}s"
        echo "  ----- output before it stopped -----"
        printf '%s\n' "$actual"
        RESULT="TIMEOUT"
        return
    fi

    # Show what the program printed (runtime errors and crashes appear here too)
    echo "  ----- program output -----"
    printf '%s\n' "$actual"
    echo "  --------------------------"
    if [ "$rc" -ne 0 ]; then
        echo "  run:   RUNTIME ERROR (make run exited with code $rc)"
    else
        echo "  run:   ok"
    fi

    # Remove the prompts and the answers typed into them, so expected.txt
    # only needs the program's real output
    if [ -f "$input" ]; then
        local line prompt answer
        while IFS= read -r line || [ -n "$line" ]; do
            [[ "$line" == *=* ]] || continue
            [[ "$line" =~ ^[[:space:]]*# ]] && continue
            prompt="${line%%=*}"; answer="${line#*=}"
            prompt="$(printf '%s' "$prompt" | sed 's/^[[:space:]]*//; s/[[:space:]]*$//')"
            answer="$(printf '%s' "$answer" | sed 's/^[[:space:]]*//; s/[[:space:]]*$//')"
            actual="$(printf '%s' "$actual" | P="$prompt" A="$answer" perl -0pe 's/\Q$ENV{P}\E[ \t]*\Q$ENV{A}\E//')"
        done < "$input"
    fi

    # Spaces, tabs and blank lines are ignored: trim each line, squeeze
    # runs of spaces to one, and drop blank lines before comparing
    local exp_norm act_norm
    exp_norm="$(normalize < "$expected")"
    act_norm="$(printf '%s\n' "$actual" | normalize)"

    if [ "$exp_norm" = "$act_norm" ]; then
        RESULT="PASS"
    fi
    echo "  output: $RESULT"
    [ "$rc" -ne 0 ] && RESULT="RUNTIME_ERROR"

    if [ "$RESULT" != "PASS" ]; then
        echo "  ----- differences (< expected   > yours) -----"
        diff <(printf '%s\n' "$exp_norm") <(printf '%s\n' "$act_norm")
    fi
}

# ---------------------------------------------------------------- student mode
if [ "${1:-}" != "-g" ]; then
    EXPECTED_FILE=""
    if [ "${1:-}" = "-e" ]; then
        [ $# -eq 3 ] || usage
        EXPECTED_FILE="$2"
        shift 2
    fi
    [ $# -eq 1 ] || usage
    [ -d "$1" ] || { echo "Error: folder '$1' not found." >&2; exit 1; }

    # Header: the git branch name if this is a git repo, otherwise the folder
    name="$(cd "$1" && git symbolic-ref --short HEAD 2>/dev/null)"
    [ -z "$name" ] && name="$(pwd)"
    GRADED="$(pwd)/graded.txt"
    SEP="================================================================"

    report="$(mktemp)"
    {
        echo "$SEP"
        echo "  Student: $name"
        echo "  Lab:     $1"
        echo "$SEP"
        grade_lab "$1"
        echo "  RESULT: $RESULT"
        echo
    } > "$report" 2>&1
    cat "$report"
    cp "$report" "$GRADED"
    rm -f "$report"
    echo "Saved: $GRADED"
    exit 0
fi

# ---------------------------------------------------------------- instructor mode (-g)
shift
GH_USER=""
GH_TOKEN=""
while getopts ":u:t:" opt; do
    case "$opt" in
        u) GH_USER="$OPTARG" ;;
        t) GH_TOKEN="$OPTARG" ;;
        *) usage ;;
    esac
done
shift $((OPTIND - 1))

[ -n "$GH_USER" ] && [ -n "$GH_TOKEN" ] && [ $# -eq 2 ] || usage
if [ "$GH_USER" != "mcrepssdi" ]; then
    echo "Error: -g is only available to the instructor." >&2
    exit 1
fi
REPO="$1"
LAB="$2"
WORK="$(pwd)/grading"
CSV="$(pwd)/results.csv"
GRADED="$(pwd)/graded.txt"

# Build an https URL with the user and token
# (accepts https://github.com/org/repo.git or git@github.com:org/repo.git)
path="$(printf '%s' "$REPO" | sed -E 's#^https://github\.com/##; s#^git@github\.com:##')"
AUTH_URL="https://$GH_USER:$GH_TOKEN@github.com/$path"

rm -rf "$WORK"
mkdir -p "$WORK"
git clone --quiet "$AUTH_URL" "$WORK/repo" || { echo "Error: could not clone $REPO" >&2; exit 1; }
cd "$WORK/repo" || exit 1
git remote set-url origin "https://github.com/$path"   # don't leave the token on disk

echo "student,result" > "$CSV"
: > "$GRADED"
SEP="================================================================"

for branch in $(git branch -r --format='%(refname:short)' | grep -v -- '->' | sed 's#^origin/##' | grep -vx main | grep -vx origin); do
    git checkout --quiet --force "origin/$branch" 2>/dev/null
    git clean -qfdx
    report="$(mktemp)"
    {
        echo "$SEP"
        echo "  Student: $branch"
        echo "$SEP"
        if [ -d "$LAB" ]; then
            grade_lab "$LAB"
        else
            echo "  no $LAB folder"
            RESULT="NO_LAB"
        fi
        echo "  RESULT: $RESULT"
        echo
    } > "$report" 2>&1
    RESULT="$(sed -n 's/^  RESULT: //p' "$report" | tail -1)"
    cat "$report"
    cat "$report" >> "$GRADED"
    rm -f "$report"
    echo "$branch,$RESULT" >> "$CSV"
done

echo
echo "Done. Review: $GRADED"
echo "      Summary: $CSV"
