# Baremetal ARM - QEMU

Simple application for testing CXX on bare-metal a target (ARM64) through QEMU. 

## Build

Note: you will need to have `qemu-system-arm` installed for this to work.

For Fedora: 
```
sudo dnf install qemu-system-arm
```
For Ubuntu:
```
sudo apt-get install qemu-system-arm 
```

```shell
$ mkdir build; cd build
$ cmake ..; cmake --build .
# Automatically load the exec binary as a kernel into qemu.
$ make run 
```

To quit the qemu-session:
```
Press `alt + a`, then `x`
```