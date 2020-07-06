# Sym Usage

Translates models from algebraic form into one of several programming
languages.

## Usage:
    sym [options] <language> <symfile> <codefile>

## Options:
    -version -calc -d -dd -doc -first -last -scalars -syntax -merge_only

## Languages:
    -debug -html -msgproc -oxgs -oxgst -oxnewton -tablo -troll 

## Details:

### Argument language
Indicates the target output language and must be one of the alternatives
listed under the Languages heading. Required unless the -merge_only option
is used.

### Argument symfile
Indicates the input model to be translated. Required.

### Argument codefile
The name that should be used for the resulting target-language file. Some
target languages will involve multiple files that will be based on this
name. Required.

### Option -calc
Turn on calculator mode for target languages that support it. Calculator
mode is used for non-iterative calculations.

### Option -d
Turn on the most commonly used debugging messages.

### Option -dd
Turn on all debugging messages.

### Option -doc
Print this message.

### Option -first
Build a single-year model using only the first year.

### Option -last
Build a single-year model using only the last year.

### Option -merge_only
Combine all included modules and return the resulting file
without generating any target-language code.

### Option -scalars
Only applies when the -debug language target is used. Causes
an additional file to be written showing element-by-element
declarations and usage of parameters and variables.

### Option -syntax
Print a short summary of the input syntax, including some
notes about rules appling to specific target languages.

### Option -version
Print detailed information about the versions of the main
program and the individual language support modules.

Sym v2.9.3 build 964 (2020/07/06)
