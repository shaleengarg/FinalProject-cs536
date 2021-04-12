# CASH
**C**ompute **A**wareness for **S**torage **H**eterogeniety 

## Table of Contents
1. [Setup](#setting-up-the-environment)
    * [Environment Setup](#setting-up-the-environment)
    * [Compiling CASH](#compiling-cash)
3. [Running CASH](#running-with-cash)
    * [Statistics Export](#exporting-cash-statistics)
    * [Example Run](#example-run)
7. [Test Applications](#test-applications)
    * [RockDB](#rocksdb)
    * [Appbench](#appbench)
8. [Debugging](#debugging)
    * [Compiling with Debugging](#compile-cash-with-debugging-output)
    * [Redirecting Debug Output](#redirecting-debug-output)
10. [List of Aliases](#complete-list-of-aliases)


## Setting up the Environment
To set up the environment to run experiments and more, navigate to the root of 
the project folder and source run the `setvars.sh` and `setaliases.sh` scripts located in the scripts directory:
```
~/CASH$ source ./scripts/setvars.sh
~/CASH$ source ./scripts/setaliases.sh
```
The `setvars.sh` script will set the following enviroment variables needed for experiments. 
The following list are environment variables that you can modify to your liking:

| Env Variable | Description |
|--------------|-------------|
| ROCKSDB_PATH | path to rocksdb src folder |
| ROCKSDB_DBPATH | path to where you want the rocksdb database to go to |
| APPBENCH_PATH | path to appbench src folder |

The `setaliases.sh` will set some command aliases to compile, configure, and run CASH:
| Command | Description |
|--------------|-------------|
| `run <application>` | run specified application with CASH |
| `set-xml-export <export path>` | set the path to export statistics as XML file. Example: `set-xml-export ./results.xml`|
| `set-run-name <name>`| set the name you want the trace to have within the exported XML file |
| `set-run-description <message>`| set the description you want the trace to have within the exported XML file |
| `compile` | compile CASH |
| `compile-debug` |  compile CASH with debugging statements |
| `clean` | clean up CASH project and compile library |


## Compiling CASH
To compile CASH, simply run the `compile` command:
```
~/CASH/src$ compile 
```
After compiling, the CASH will be compiled into *shim.so* which you will be able to ld_preload.

## Running with CASH
To run an application with CASH, use the `run` command to run you application:
```
~/CASH$ run <application>
```

## Exporting CASH statistics
CASH collects statistics about file operations. It collects statistics on a per-process basis. Each process collects stats on which threads do file operations as well as (1) which files they touch (2) which operations they use, and (3) how many bytes read/written (if relavent to the operation). By default, each process will print their collected statistics when they terminate. However, you can export these statistics by setting the `XML_EXPORT` environment variable with the `set-xml-export` command:
```
~/CASH$ set-xml-export <path>
```
Example:
```
~/CASH$ set-xml-export ./db_bench-results.xml
```

Example XML format:
```
<run name="db_bench" description="First run of db_bench.">
    <processes>
        <process id="38656">
            <threads>
                <thread id="38656" name="db_bench">
                    <files>
                        <file name="/home/CASH/rocksdata/LOG">
                            <operation name="fwrite">
                                <stat name="bytes">3</stat>
                                <stat name="calls">3</stat>
                            </operation>
                        </file>
                        <file name="/home/CASH/rocksdata/WAL_LOG/000004.log">
                            <operation name="write">
                                <stat name="bytes">32467826</stat>
                                <stat name="calls">7692</stat>
                            </operation>
                            <operation name="fallocate">
                                <stat name="calls">1</stat>
                                <stat name="length">73819750</stat>
                            </operation>
                        </file>
                    </files>
                </thread>
            </threads>
        </process>
    </processes>
    <ignored_files>
        <ignored_file name="/dev/pts/17"></ignored_file>
        <ignored_file name="/proc/cpuinfo"></ignored_file>
        <ignored_file name="/proc/sys/kernel/random/uuid"></ignored_file>
    </ignored_files>
</run>
```

## Example Run:
Here is an example of running db_bench with CASH. Make sure to set the environment variable and aliases before doing the following:
```
~/CASH$ set-xml-export db_bench-results.xml
~/CASH$ set-run-name "db_bench"
~/CASH$ set-run-description "First run of db_bench."
~/CASH$ run db_bench 
[CASH] Exporting statistics results to /home/djd240/CASH/db_bench-results.xml
[CASH] Run name set to db_bench
[CASH] Run description set to First run of db_bench.
[CASH] Application Starting. Initiating tracing...
....
[CASH] Application ending. Tracing terminating...
[CASH] Exporting statistics to /home/djd240/CASH/db_bench-results.xml
[CASH] Sucessfully exported statistics.
```

## Test Applications
To clone test applications you want to test CASH with, you can use the application scripts in the
scripts directory

#### RocksDB
All RocksDB management can be done with the *scripts/rocksdb.sh* script. This script accepts the following arguments:
- **clone** - clone RocksDB repo to $ROCKSDB_PATH
- **compile** - compile RocksDB with db_bench
- **compile-debug** - compile RocksDB and db_bench with debugging on
- **clean** - clean the RocksDB folder
- **remove** - remove the RocksDB repo all together

#### AppBench
AppBench is a repository which hold multiple applications that can be used for benchmarks. This includes:
1. GraphChi
2. GTC-benchmark
3. Metis
4. Redis

All AppBench management can be done with the *scripts/appbench.sh* script. This script accepts the following arguments:
- **clone** - clone AppBench repo to $APPBENCH_PATH
- **compile** - compile RocksDB with db_bench
- **remove** - remove the AppBench repo all together

## Debugging
### Compile CASH with debugging output
To compile CASH with debugging output run the `compile-debug` command:
```
~/CASH/src$ `compile-debug`
```
### Redirecting Debug Output
By default, CASH's debugging output will print out to STDERR. However you can redirect this by setting the `DEBUG_OUTPUT` environment variable.
#### Redirecting debugging output to a file
Redirecting debugging output to a file is as easy as setting the `DEBUG_OUTPUT` environment variable with the `set-debug-output` command to the file you want the output to be written to:
```
~/CASH$ set-debug-output ./output.txt
```

#### Redirecting output to another terminal
There may be cases where you want to monitor the debugging output as an application runs without interfering with the original application's output. You can do this by having two terminal windows open and have CASH redirect its debugging output to the second terminal window. To do this:

In the second terminal window, figure out the terminal device file with *tty*:
```
~/CASH$ tty
/dev/pts/3
```

Now set the device file as the debug output in the first terminal where you will run your application:
```
~/CASH$ set-debug-output /dev/pts/3
```
At this point, CASH will redirect all debugging output to the second terminal window.


## Complete List of Aliases
The following is a complete list of aliases set when you source run the `setaliases.sh` script located in the scripts directory:

#### CASH Aliases
| Alias | Description |
|--------------|-------------|
| `run` | run some program with CASH shim. Simply a shortcut to LD_Preload shim library before running command/program/script. **Example**: `$ run db_bench`|
| `compile` | compiles CASH |
| `compile-debug` | compile CASH with debugging on |
| `clean` | cleans CASH project |

#### Environment Variable Setting Aliases
| Alias | Description |
|--------------|-------------|
| `set-debug-output` | sets DEBUG_OUTPUT env variable. **Example**: `$ set-debug-output /dev/pts/3` |
| `unset-debug-output` | unsets DEBUG_OUTPUT env variable |
| `set-xml-export` | sets XML_EXPORT env variable. **Example**: `$ set-xml-export stats.xml`|
| `unset-xml-export` | unsets XML_EXPORT env variable. |

#### XML Manipulation Aliases
| Alias | Description |
|--------------|-------------|
| `xml-aggregate` | aggregate xml files with prefix $XML_EXPORT or specified prefix(es) via xml.sh script |
| `xml-clean` | remove (clean) xml files with prefix $XML_EXPORT or specified prefix(es) via xml.sh script |

#### RocksDB Aliases
| Alias | Description |
|--------------|-------------|
| `rocksdb-clone` | clones RocksDB repo via rocksdb.sh script |
| `rocksdb-compile` | compiles RocksDB and db_bench via rocksdb.sh script |
| `rocksdb-compile-debug` | compiles RocksDB and db_bench with debugging enabled via rocksdb.sh script |
| `rocksdb-clean` | cleans RocksDB repo via rocksdb.sh script |
| `rocksdb-remove` | removes RocksDB repo via rocksdb.sh script |


#### DB_Bench Aliases
**Note:** requires RocksDB to be compiled for these to work
| Alias | Description |
|--------------|-------------|
| `db_bench` | alias to db_bench executable |
| `db_bench-clean` | cleans up db_bench database |


#### AppBench Aliases
| Alias | Description |
|--------------|-------------|
| `appbench-clone` | clones AppBench repo via appbench.sh script |
| `appbench-compile` | compiles AppBench and db_bench via appbench.sh script |
| `appbench-remove` | removes AppBench repo via appbench.sh script |

#### AppBench Benchmark Aliases
**Note:** requires AppBench to be compiled for these to work
| Alias | Description |
|--------------|-------------|
| `graphchi` | alias to graphchi ./run script |
| `gtc` | alias to gtc-benchmark ./run script |

#### System Aliases
| Alias | Description |
|--------------|-------------|
| `clear-cache` | clears the system cache |
