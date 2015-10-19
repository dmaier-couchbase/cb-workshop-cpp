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
yum groupinstall 'Development Tools'
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

* Day 2: Using the Couchbase C Client Library

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
| 17:00           | 2.7           | Q&A, Summary, Outlook                   | What's new in 4.0? |


## Exercises

### Day 1: Couchbase Architecture and Administration Basics

| #               | Title                                  | Content                                      | 
| --------------- | -------------------------------------- | -------------------------------------------- |
| 1               | Installation and Configuration         | Disable Swappines | 
|                 |                                        | Disable the Linux Firewall |
|                 |                                        | Download and Install Couchbase |
|                 |                                        | Configure the Cluster |
| 2               | Testing the Installation               | List the nodes of your current cluster |
|                 |                                        | Investigate the data and index directory |
|                 |                                        | Get some data from a vBucket file |
|                 |                                        | Get some info about a vBucket file |
|                 |                                        | Install Telnet |
|                 |                                        | Retrieve some statistics via Telnet |
|                 |                                        | Set/get a value via Telnet |
|                 |                                        | Install Curl |
|                 |                                        | Get details via the REST API |
| 3               | Working with Buckets                   | Create a Bucket via the UI |
|                 |                                        | Add a document to the Bucket |
|                 |                                        | Create a Bucket via the CLI|
| 4               | Working with the Cluster               | Add/remove nodes via the UI|
|                 |                                        | Rebalance|
|                 |                                        | Add/remove nodes via the CLI |
| 5               | Backup/Restore                         | Use cbbackup to backup a Bucket|
|                 |                                        | Use cbrestore to restore to another Bucket|
| 6               | XDCR                                   | Create an XDCR link via the UI |

### Day 2: Using the Couchbase C Client Library

The starting point for the day 2 execises is the 'TravelAppSample-Empty' application. This is basically an application skeleton which has:

* All the header files
* An empty implementation of the required methods
* Test/demo cases

The application 'TravelAppSample' then contains all the exercise solutions.


| #               | Title                                  | Content                                      | 
| --------------- | -------------------------------------- | -------------------------------------------- |
| 7               | Managing Connections                   | Download and install libcouchbase | 
|                 |                                        | Implement the Connect method in CBDataSource |
|                 |                                        | Implement the CBDataSourceFactory |
| 8               | Create/Update a Document               | Implement the Upsert method in CBDataSource |
| 9               | Get a Document                         | Implement the Get method in CBDataSource |
|                 |                                        | Implement the GetJsonObject method in CBDataSource |
| 10              | Delete a Document                      | Implement the Delete method in CBDataSource |
| 11              | Perform a Multi-Get                    | Implement the MultiGet method in CBDataSource |
| 12              | Query a View                           | Create a View via the UI |
|                 |                                        | Implement the QueryView method in CBDataSource |
| 13              | Query via N1QL                         | Inspect the Global Secondary Indexes |
|                 |                                        | Implement the QueryN1ql method in CBDataSource |
| 14              | A Sample Application                   | Run the Qt Travel-Sample application |
|                 |                                        | Perform a flight search from 'LAX' to 'LHR' |

## Qt Travel-Sample Application

![alt tag](https://raw.githubusercontent.com/dmaier-couchbase/cb-workshop-cpp/master/resources/screenshots/Log-in.png)
![alt tag](https://raw.githubusercontent.com/dmaier-couchbase/cb-workshop-cpp/master/resources/screenshots/Search.png)
![alt tag](https://raw.githubusercontent.com/dmaier-couchbase/cb-workshop-cpp/master/resources/screenshots/Available-Flights.png)
![alt tag](https://raw.githubusercontent.com/dmaier-couchbase/cb-workshop-cpp/master/resources/screenshots/Shopping-Cart.png)


## Help

* Where can I find the View for 'airports/by_name'?

The View code can be found in the 'resources/views' sub-folder. You need to create a Design Document called 'airports' in your 'travel-sample' bucket. Then a View 'by_name' needs to created in this Design Document. You should test the View via the UI before doing the related exercise. 

* How to install the Sample Data?

In case that you didn't install the sample data together with Couchbase 4.0 the following shows how to load it:

```
cbdocloader -u couchbase -p couchbase -n 127.0.0.1:8091 -b travel-sample -s 128 travel-sample.zip
```
