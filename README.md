# Couchbase C/C++ Workshop

This page contains details how to use the workshop material.

## Requirements

For the first day each attendee needs to have prepared the following:

* 3 VM-s with CentOS 6
* Static IP addresses
* A ping between the VM-s needs to be possible

For the second workshop days the following is required:

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

## Help

## How to install the Sample Data

cbdocloader -u couchbase -p couchbase -n 127.0.0.1:8091 -b travel-sample -s 128 travel-sample.zip

