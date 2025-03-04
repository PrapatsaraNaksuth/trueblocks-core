chifra argc: 2 [1:-th] 
chifra -th 
TIME ~ CLOCK - <ERROR> : Exit(chifra,call_command): 
#### Usage

`Usage:`    chifra command  
`Purpose:`  Access to all TrueBlocks tools (`chifra <cmd> --help` for more).

`Where:`  
```
 MONITORS
   list          list every appearance of an address anywhere on the chain
   export        export details for each appearance (as txs, logs, traces, balances, reconciliations, etc.)
   monitor       add, remove, clean, and list appearances of address(es) on the chain
 SHARED DATA
   entities      list and/or share entities (groups of addresses)
   names         list and/or share named addresses
   tags          list and/or share tags (subgroups of addresses)
   abis          list and/or share abi signatures
 BLOCKCHAIN DATA
   blocks        export block-related data
   transactions  export transaction-related data
   receipts      export receipt-related data
   logs          export log-related data
   traces        export trace-related data
   state         export parts of the state for given address(es)
   tokens        export data related to ERC20 and/or ERC721 token(s)
   when          return a date given a block number or a block number given a date
 ADMIN
   init          initialize TrueBlocks databases by downloading pinned bloom filters
   scrape        scrape the chain and build an index of address appearances (aka digests)
   serve         serve the TrueBlocks API via tbServer
   pins          query the status of the pinning system
   status        query the status of the system
 OTHER
   explore       open the configured block explorer for the given address
   slurp         export details by querying EtherScan (note: will not return as many appearances as --list)
   quotes        return prices collected from configured remote API
   where         determine the location of block(s), either local or remote cache, or on-chain
```

