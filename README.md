# Couchbase C/C++ Workshop

This page contains details how to use the workshop material.

## Requirements

For the first day each attendee needs to have prepared the following:

* 3 VM-s with CentOS 6
* Static IP addresses
* A ping between the VM-s needs to be possible

For the second workshop day the following is required:

* A C/C++ Development environment which especially includes 'gcc' and 'make'

```
# RHEL6/EPEL
yum install ‚Development Tools'
```

* Qt 5 installed

```
# RHEL6/EPEL
yum install qt
```

* A C/C++ IDE of your choice is installed

```
# RHEL6/EPEL
yum install qt-creator
```

* The IDE needs to be configured to support the Qt libraries
* A Git client needs to be installed

## Agenda

* Day 1: Couchbase Architecture and Administration Basics

| Time            | Chapter       | Title                                   | Content                                          |
| --------------- | ------------- | --------------------------------------- | ------------------------------------------------ |
| 09:00           | 1.1           | Introduction and Core Use Cases         | What is Couchbase Server?                        |
|                 | 1.2           | Couchbase Server Architecture           | Caching Layer, Storage Engine, CM                |
|                 | 1.3           | Couchbase Server as a Distributed System| Intra-Cluster Replication, Consistency, Sharding |
| 10:30           |               | Coffee Break                            |                                                  |
|                 | 1.4           | Working with Buckets                    | Create Bucket, add Data                          |
|                 | 1.5           | Working with the Cluster                | Add/remove Nodes, Server Groups, Rebalance       |
| 12:30           |               | Lunch                                   |                                                  |
|                 | 1.6           | Security                                | Encrytion, LDAP Integration, Configuration       |
|                 | 1.7           | Cross Data Center Replication explained | Setup, Configuration Parameters                  |
| 17:00           |               | Q&A and Summary                         |                                                  |

* Day 2: Using the Couchbase C SDK

| Time            | Chapter       | Title                                   | Content                                          |
| --------------- | ------------- | --------------------------------------- | ------------------------------------------------ |
| 09:00           | 2.1           | Managing Connections                    | Params, Client Handle, Open/Close a Bucket, SSL  |
|                 | 2.2           | Understanding Non-Blocking I/O          | Scheduling Operations, Callbacks                 |
| 10:30           |               | Coffee Break                            |                                                  |
|                 | 2.3           | Working with Documents                  | Document Modelling, CRUD Ops, Bulk Ops, Incr     |
|                 | 2.4           | Error Handling and Logging              | Error Codes, Setting up Logging                  |
| 12:30           |               | Lunch                                   |                                                  |
|                 | 2.5           | Develop a Sample Application            | Qt Travel Sample                                 |
|                 | 2.6           | Specific Use Case presentation          |                                                  |
| 17:00           | 2.7           | Q&A, Summary, Outlook                   | Documentation, Examples, Tutorials, RoadMap      |


## Exercises

| #               | Title                                  | Content                                      | 
| --------------- | -------------------------------------- | -------------------------------------------- |
| 1               | Installation and Configuration         | Disable Swappines | 
|                 |                                        | Disable the Linux Firewall |
|                 |                                        | Download and Install Couchbase |
|                 |                                        | Configure the Cluster |



### 2 - Testing the Installation

* Perform the following steps in order to test your installation
  * List the nodes of your current cluster
  * Investigate the data and index directory
  * Get some data from a vBucket file
  * Get some info about a vBucket file
  * Install Telnet
  * Retrieve some statistics of the default bucket via Telnet
  * Set/get a value via Telnet
  * Install Curl
  * Get details via the REST API

### 3 - Working with Buckets

* Perform the following steps in order to create a bucket
  * Create a Bucket via the UI
  * Add a document to the Bucket
  * Create a Bucket via the CLI

### 4 - Working with the Cluster

* Perform the following steps in order to add/remove a node 
To/from the cluster
  * Add/remove nodes via the UI
  * Rebalance
  * Add/remove nodes via the CLI

### 5 - Backup/Restore

* Perform the following steps in order to backup some data
  * Use cbbackup
  * Use cbrestore

### 6 - XDCR

* Perform the following steps in investigate the XDCR part 
of the Admin UI
  * Create an XDCR link



## Help

## How to install the Sample Data

cbdocloader -u couchbase -p couchbase -n 127.0.0.1:8091 -b travel-sample -s 128 travel-sample.zip

