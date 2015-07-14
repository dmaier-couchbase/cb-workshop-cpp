# Couchbase C/C++ Workshop

This page contains details how to use the workshop material.

## Requirements

For the first day the following needs to be prepared:

* 3 VM-s with CentOS 6
* Static IP addresses
* A ping between the VM-s needs to be possible

For the second workshop days the following is required:

* A C/C++ Development environment which especially includes 'gcc' and 'make'

```
# RHEL/EPEL
yum install ‚Development Tools'
```

* Qt 5 installed

```
# RHEL/EPEL
yum install qt
```

* A C/C++ IDE of your choice is installed

```
# RHEL/EPEL
yum install qt-creator
```

* The IDE needs to be configured to support the Qt libraries


## Install the Sample Data

cbdocloader -u couchbase -p couchbase -n 127.0.0.1:8091 -b travel-sample -s 128 travel-sample.zip
