# Bench Parser #

Parser for the ISCAS85/89/99 bench file format. It parsers the bench files aiming at generating an abstract syntax graph (ASG). The nodes of the ASG are topologically sorted in order to make the calls for the BDD package.

## Installation
### Step 1: Clone the directory

Run the git clone command replacing *<username>* by you username.
```
git clone https://<username>@bitbucket.org/cpnogueira/bench_parser.git
```

### Step 2: Compiling
To compile it, after cloning it from bitbucket, you should be in the root folder of the project and run the command:  
```
make
```

To clean files produced during compilation (object files, executables, libraries, ...) type:
```
make clean
```

## Running the executable
The different designs for the bechmarks are located in the directory "benchmarks"
For example, to run the code for the circuit s420.bench of the iscas89 benchmark type:
```
./bench_parser benchmarks/iscas89/s420.bench
```

This will produce the subfolder results_s420 containing the results.

## API Reference
To generate or update the documentation, you should navigate to the doc/ directory and then:
```
../bench_parser/doc/$ doxygen doxyConfigEx
```
The documentation will be available on *../bench_parser/doc/doxygen/* and you can access it from the same location opening the file *index.html*.