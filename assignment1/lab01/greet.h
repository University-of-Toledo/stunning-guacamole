/*
 * greet.h
 * CSET 3150 - Lab 1 demo (Hello and the Toolchain)
 * Public interface for the greeting module used to demonstrate splitting
 * a program across multiple source/header files.
 */
#ifndef GREET_H
#define GREET_H

/* Prints a greeting for the given name to stdout, followed by a newline.
 * name must be a non-NULL, null-terminated string. */
void greet(const char *name);

#endif /* GREET_H */
