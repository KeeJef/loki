# Service node registration and staking contribution

Addresses and pubkeys is this guide are abbreviated. So the address L7q6DxQGnP6PQFe5bkV6DaGbgUj7TK7PG5dKv45mzTM6jQmoDbmZu7gfgVfnqp9yoSCtCd8GQEFLD7TKZu8UGL8335UuJ4Z would appear as thus:

    L7q6DxQGnP6P...

And the service node pubkey 3b7ba0f1ab253dcef33fd131c7482a2767740ca6f5e34300175186225a5f5eb3 would appear as thus:

    3b7ba0f1ab25...

## Overview

Service node daemons are launched with the `--service-node` flag as thus:

    ./lokid --service-node

Service node staking and registration is generally done in two parts:

1. First the service node operator registers the service node and provides
   their own contribution.
2. After the registration is in a block, the other contributors may make their
   contributions on the blockchain.

Each contributor must send at least 25% of the total staking requirement,
unless there is less than 25% remaining to be filled.

The service node operator should specify any addresses that are supposed to
participate in this service node in order to reserve their place.

To use this guide you will need to use the Loki CLI wallet. To access the CLI wallet, open a terminal/command prompt window and use the CD (Change directory) command to navigate to the folder you downloaded your CLI wallet to.

If you are reading this guide while Service Nodes are being tested on testnet, you will need to **add the --testnet flag to your daemon and wallet on startup.**

the Daemon and wallet can be started by using the command in terminal ./lokid (Daemon) and ./loki-cli-wallet (Wallet) if you are using Windows you can omit the ./

## Basic syntax

The service node operator runs the following command from the service node:

    ./lokid --prepare-registration <operator cut> <address> <fraction> <address2> <fraction2> ... <contribution amount>

The address/fraction pairs are to reserve a portion of the service node reward for address1, address2, ..., etc. where fraction is between 0.25 and 1.
The first address is always the service node operator's address. The operator cut is the amount reserved for the pool operator.

The contribution address and amount means that you will send `<contribution amount>` funds in a locked transfer for 30 days worth of blocks.

### Basic Example

To do a basic registration of a single contributor (you), you would run this:

    ./lokid --prepare-registration 0 L7q6DxQGnP6P... 1 35000.0

This means the given address will receive 100% of the rewards, and the wallet will send 35000.0 to that address in a locked transfer. Note that in testnet, the stake amount is statically set at 100 Loki and the registration period is 2 days.

This will generate a signed command that is valid for two weeks to run in the CLI wallet:

    [wallet L7q6Dx]: register_service_node 0 L7q6DxQGnP6P... 1 35000.0 1534842024 3b7ba0f1ab25... f057d1e74193ef66b4cf63fe88d4e0b287db78cf807ad0d5b9e4d29ec89b2e03021f6bfc4369ab18288acb390c082338ac78ed39fd1a6c03a30acdce0bcb3205

### Shared Example

To do a basic registration with two participants, you would run this:

    ./lokid --prepare-registration 0 L7q6DxQGnP6P...  0.5 L514yZNZuTHb... 0.5 17500.0

In this example, each of the two specified addresses will have 50% of the staking contribution reserved for them. The first address is specified for the locked transaction of 17500.0 at the end.

This will generate a signed command that is valid for two weeks to run in the CLI wallet.

After this transaction is in the blockchain, the second contributor may contribute their portion of the service node staking amount from the CLI like this:

    [wallet L514yZ]: stake 3b7ba0f1ab25... L514yZNZuTHb... 17500.0

Where `<pubkey>` is the service node public key and the address specified is their own address, as it appears in the registration.

### Pooled example

To do a basic registration with one guaranteed participant of 25%, and an open number of pooled participants, you would run this:

    ./lokid --prepare-registration 0 L7q6DxQGnP6P... 0.25 8750.0

This means reserve 25% for the specified address, then send a 30-day locked transfer of 8750.0 to the specified address.

Then participants who want to contribute to this pool can do so with this command in the CLI:

    [wallet L514yZ]: stake 3b7ba0f1ab25... L514yZNZuTHb... 26500.0

Where `<pubkey>` is the public key for the service node and `<amount>` is the amount they want to contribute.

### Operator costs

Optionally, the registration can include a portion of the payout to the service node operator, irrespective of relative contributions. For this, change the first argument after --prepare-registration to the cut reserved for the service node operator costs.

e.g. to reserve 30% to the operator, and split the remaining parts equally between two contributors, you use this command:

    ./lokid --prepare-registration 0.33 L7q6DxQGnP6P...  0.5 L514yZNZuTHb... 0.5 17500.0
