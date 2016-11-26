# PThreadProdCons
![](http://posix.co.uk/wp-content/uploads/2015/06/Web-Logo2-300x138.png)

Linux Systems:
to compile this code, set the working directory into the root project and use this command

```sh
$ gcc src/buffer_type.c src/msg_type.c test/test.c -o exe -lcunit -pthread
```

to execute then use this one


```sh
$ ./exe
```