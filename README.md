# VLSI Testing Final Project
- Topic: N-detect TDF ATPG and Compression

# TDF-pattern generation and fault simulation
## TDF ATPG
```
    $ ./atpg -tdfatpg -ndet <num> -compression ../sample_circuits/<circuit_name>.ckt > ../tdf_patterns/<circuit_name>.pat
```
- -ndet is for n-detect ATPG
- -compression is whether to perform test compression
## TDF fault simulation
```
    $ ./atpg -tdfsim ../tdf_patterns/<circuit_name>.pat ../sample_circuits/<circuit_name>.ckt
```

# Using Git
## Create a branch
``` 
 $ git branch <branchname>
```
- Create and go to the branch directly
```
 $ git checkout -b <branchname>
```
## Go to the branch
```
$ git checkout <branch> 
```
## List all branchs
```
 $ git branch
```
## Add modified files
- Add all files
```
 $ git add .  
```
- Add file by name
```
 $ git add \<file1> \<file2> ... 
```
## Make a commit
```
 $ git commit -m "commit message"
```
## Merge branch
```
 $ git checkout main
```
- Remember to pull form remote before merge
```
 $ git pull 
 $ git merge <branch>
```
## Push to remote
```
 $ git push
```
## Delete branch
```
 $ git branch -d \<branch>
```
## Reference and more detail
- https://backlog.com/git-tutorial/tw/stepup/stepup1_1.html
- https://hackmd.io/dKv9yBe-Rz-Cv9zDvX9f1w?fbclid=IwAR0MYimlom981JhKOiNj6rilEJZ1aXis7zYNQ6IOjeFdae9erQuyI1vcpp8
- QAQ
