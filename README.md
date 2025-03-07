# Welcome to My Blockchain
***

## Task
Creating a mini blockchain project that has nodes and inside of them blocks.

## Description
It is a simple project that has similar behaviors as real blockchain. You can perform actions like adding blocks or nodes or synchronise them on it.

## Installation
There is nothing to install. Simply download it and go on using it.

## Usage
You write a command make and it creates a target called "my_blockchain". You run it and give it appropriate commands
<br><br>The commands are:<br>
    <b>add node</b> <i>nid</i> | add a nid identifier to the blockchain node.
    <b>rm node</b> <i>nid</i>  | remove nodes from the blockchain with a nid identifier. If nid is '*', then all nodes are impacted.
    <b>add block</b> bid nid... add a bid identifier block to nodes identified by nid. If nid is '*', then all nodes are impacted.
rm block bid... remove the bid-identified blocks from all nodes where these blocks are present.
ls list all nodes by their identifiers. The option -l attaches the blocks' bids associated with each node.
sync synchronize all of the nodes with each other. Upon issuing this command, all of the nodes are composed of the same blocks.
quit save and leave the blockchain.
```
./my_blockchain
```

### The Core Team


<span><i>Made at <a href="https://qwasar.io">Qwasar SV -- Software Engineering School</a></i></span>
<img alt="Qwasar SV -- Software Engineering School's Logo" src="https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png" width="20px">
