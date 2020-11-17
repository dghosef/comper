# Your CFGs must be in the following format: 
```html % Use html for markdown regex representation because of syntax highlighting inside <.*>
<nonterminal1> = terminal weight | <nonterminal2> weight
<nonterminal1> = <nonterminal2>terminal<nonterminal2> weight
<nonterminal2> = <nonterminal1>
```





Note: 
- An epsilon is automatically appended to the end of each line but is only invoked if our generated sentence is of the required length. 
- The weight is always the last number before the | or newline
- Every nonterminal without a weight is automatically assigned a weight of 1.
- You may only have the symbols a-z, A-Z, 1-9, , _, +, and - for terminal strings and for nonterminal names
- The first state in our CFG is the start state.
- PickNext from CFG
- Extend CFG to have more memory?
