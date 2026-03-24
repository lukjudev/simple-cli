# Command Line Interpreter
A small command-line interpreter in C++: lexer, parser, pipelines, redirections, and built-in commands.

## Command reference

Commands are case-sensitive. Tokens are separated by spaces or tabs. Text in double quotes `"..."` is one argument (any characters except `"` and newline).

| Command | Format | Description |
|--------|--------|-------------|
| **echo** | `echo` or `echo` *argument* | Copies input stream to output; with one argument, reads from quoted text or a file path. |
| **prompt** | `prompt` `"text"` | Sets the interactive prompt string (argument must be quoted). |
| **time** | `time` | Prints current time. |
| **date** | `date` | Prints current date. |
| **touch** | `touch` *filename* | Creates an empty file; errors if it already exists. |
| **truncate** | `truncate` *filename* | Clears file contents. |
| **rm** | `rm` *filename* | Deletes a file. |
| **wc** | `wc` `-w` \| `-c` [ *argument* ] | `-w` counts words, `-c` counts characters. Optional argument: quoted text or file path; otherwise input comes from stdin. |
| **tr** | `tr` `-` `"what"` [ `"with"` ] or `tr` *src* `-` `"what"` [ `"with"` ] | Replaces all occurrences of `"what"` with `"with"` in the input text; omit `"with"` to remove matches. First form uses stdin; second uses quoted literal or file *src* before `-`. |
| **head** | `head` `-n`*count* [ *argument* ] | Prints the first *count* lines (digits directly after `-n`, up to 5 digits). Optional argument: quoted text or file; else stdin. |
| **batch** | `batch` *filename* | Runs each non-empty line of the file as its own command line. |

## Pipelines

- Use `|` between commands: **stdout** of the left command becomes **stdin** of the right command.
- Example: `time | tr - ":" "." | wc -c`
- **time** and **date** may appear only as the **first** command in a pipeline.
- **touch**, **rm**, **truncate**, and **prompt** must be the **last** command in a pipeline (they do not produce a normal stream for the next stage).
- If a command already takes input from an argument (file or quoted text), it cannot appear after `|` with conflicting input semantics; the parser reports a semantic error.

## Redirections

Redirection tokens must appear only at the **end of the whole command line** (after all commands and pipes).

| Operator | Meaning |
|----------|---------|
| `<` *file* | **Input** for the pipeline is read from *file* (feeds the **first** command). |
| `>` *file* | **Output** of the pipeline is written to *file* (truncates if the file exists). |
| `>>` *file* | Same as `>`, but **appends** to *file*. |

Optional spaces/tabs may appear around `<`, `>`, and `>>`. You may combine input and output redirection at the end of the line; their order is flexible.

A command that already defines stdin via an argument cannot be combined with a line-level `<` redirect (semantic conflict).

## Limits

- One input line is at most **512** characters (excluding the newline); the rest is discarded.