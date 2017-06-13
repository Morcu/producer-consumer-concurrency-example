# Producer-consumer-concurrency-example
Producer consumer example: a concurrent Stock market

From the stock file and the batch operation files the Stock market will work.

**_In order to get a correct execution of the stock market, mutex concurrency is used_**

## Stock

The stock file follows this configuration:



| Id            | Name          |  Number of shares     | price/share  |
| --------------|:-------------:| ---------------------:|-------------:|

- **Id**: unambiguous identifier of the company up to 10 characters.
- **Name**: A full company name of up to 255 characters.
- **Number of Shares**: The initial number of company shares.
- **Price/Share**: The initial value of each share of the company (the total value of the company will be: Number of shares * share account).



## Batch

The batch file follows this configuration:



| Id            | Type          |  Number of shares     | price/share  |
| --------------|:-------------:| ---------------------:|-------------:|

- **Id**: unique identifier of the company on which the transaction is to be applied (up to 10 characters). The company must exist in the stock market on which the broker works.
- **Type**:type of transaction: purchase or sale. 0 corresponds to the value of the BUY constant (stock purchase operation) and 1 corresponds to the constant SALE (stock sale operation).
- **Number of shares**:  The number of shares of the company to be bought or sold.
- **Price/Share**: price to be paid for each of the shares that are bought / sold. Purse

## Compiling and execution

1. make
2. ./concurrent_market
