PoC using livepatch kernel infrastructure to block SOCK_RAW socket creating and sending.


# Compiling
```bash
make
```

# Using
```bash
$ # Before. Raw sockets work
$ cat /dev/zero | sudo socat STDIN IP-SENDTO:10.0.2.16:42
^C
$ # Insert module
$ sudo insmod ./livepatch-cooked.ko
$ # Raw socket sendmsg() now fails
$ cat /dev/zero | sudo socat STDIN IP-SENDTO:10.0.2.16:42
2018/10/05 20:20:24 socat[8759] E socket(2, 3, 42): Operation not supported

$ # Unload
$ echo 0 | sudo tee -a /sys/kernel/livepatch/livepatch_cooked/enabled
$ sudo rmmod livepatch_cooked
$ # It works again
$ cat /dev/zero | sudo socat STDIN IP-SENDTO:10.0.2.16:42
^C
```
