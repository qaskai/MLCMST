### Dependencies
- CMake >= 3.16
- [Catch2](https://github.com/catchorg/Catch2 "Catch2") (downloaded automatically by CMake)

### Build
When in project root run the following.
```bash
cmake -S . -B build
cmake --build build
```
Application binaries will be placed in `./build/app/`, test binaries go to `./build/tests/`.

When new files are added to the project, `cmake -S . -B build` always needs to be run, as CMake makes a file list in configure time and only tracks changes in those files. When files are only updated this command can be skipped.

### Testing
Tests can be run through Catch binary `./build/tests/MLCMST_tests` - [option docs](https://github.com/catchorg/Catch2/blob/master/docs/command-line.md#top "option docs"). Another option is to run them with CMake - `cmake --build build -t test`.

## Features

### Done
- project structure
- CMake build
- Catch2 integration
- basic data generation
- basic data serialization

### Todo
- tests for existing code
- integration with [Google LP solver ](https://developers.google.com/optimization/mip/integer_opt "Google LP solver ")
- benchmark application
- data generation application
- preparation of set benchmark datasets
- implementation of MLCMST heuristics and LP folmulations (various), [this](https://terpconnect.umd.edu/~raghavan/preprints/mlcmst.pdf "this") will go first
