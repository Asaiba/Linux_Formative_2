# System Programming Tasks in C

This repository contains four system programming tasks written in C. Each task demonstrates core operating system concepts such as process control, file I/O, multi-threading, and performance analysis using `strace`.

---

## 🔧 Task 1: Fork, Exec, Wait & File I/O

**Objective:**  
Create a child process that executes a system command (e.g., `ls`) using `fork()`, `execvp()`, and `wait()`. Meanwhile, the parent process creates a temporary file, writes to it, and reads from it.

**Key Concepts:**  
- Process creation (`fork()`)
- Executing new programs (`execvp()`)
- Synchronization (`wait()`)
- File operations (`open()`, `write()`, `read()`)

**Command to trace:**
```bash
strace ./q1_exec
```

## Task 2: Logging Utility – Buffered vs Unbuffered I/O
Objective:
Implement two logging versions to write 1,000,000 timestamped entries to a file:

Unbuffered version using write()

Buffered version using fprintf()

Comparison Metrics (using strace -c):

Number of system calls

Execution time

System call overhead

Commands to trace:

```bash
strace -c ./log_unbuffered
strace -c ./log_buffered
```
Expected Results:
Buffered I/O should yield fewer system calls and better performance due to internal buffering.

## Task 3: Multithreaded Palindromic Counter
Objective:
Count all palindromic numbers between 0 and 100,000 using 20 threads.

Key Concepts:

Thread creation (pthread_create)

Synchronization (pthread_mutex)

Range partitioning

Expected Output:
The synchronized total number of palindromic numbers from 0 to 100000 is <total>


## Task 4: Multi-threaded Log File Merger
Objective:
Merge multiple log files into a single output file using multiple threads, ensuring thread-safe file writes.

Usage Format:

```bash
./merge merged_log.txt log1.txt log2.txt ... <num_threads>
```

Dependencies
To compile the programs, use gcc with pthreads
