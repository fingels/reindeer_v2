To make this prototype work.

# Dependencies

For Google performance analysis:   
```
sudo apt-get install -y libgoogle-perftools-dev
sudo apt-get install -y libgoogle-perftools4
sudo apt-get install -y google-perftools 
```

# Usage

Compile the sources with `make`. Then run with

```
./main --folder <path/to/folder> -k <value> -m <value> [--cap <value>] [--display <boolean>] 
```
where `folder` containing all FASTA files to be read (and nothing else), `k` the size of k-mers, `m` the size of minimizers, `cap` the number of lines from the FASTA files to read (`cap` is optional, and if not provided, all lines will be read) and `display` whether or not to display the comments during the execution. By default, they are shown.

 `--folder` can be shortened to `-f`, `--cap` to `-c` and `--display` to `-d`. Parameters can be given in any order.

**Important:** 
In case of successive runs, delete the `tmp` folder before running the next one. 