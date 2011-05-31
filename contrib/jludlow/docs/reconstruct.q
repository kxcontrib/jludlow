/ This script will join and reconstruct a global book from a range of sources
/ usage: q reconstruct.q

/##################################
/Create random quotes. 

sym:`a`b`c`d`f
src:`NYSE`LSE`NASDAQ`PAR 
n:10000
quote:([]time:`time$();sym:`symbol$();src:`symbol$();bid:`float$();ask:`float$();id:`int$())
`quote insert (n?.z.T;n?sym;n?src;n?10.;10.+n?1.;til n);
`time xasc `quote;

/###################################
/"Align" the quotes now.

book:fills{![x;enlist(=;`src;enlist y);0b;(`$'("bid_";"ask_";"time_";"id_"),\:string y)!`bid`ask`time`id]}/[quote;asc distinct quote`src]

/###################################
/Possibly add on a top of book bid and ask?

/Get the bid and ask column names
bc:cols[book] where cols[book] like "bid*"
ac:cols[book] where cols[book] like "ask*"
/Update and add new column based on bc and ac
![`book;();0b;(`topbid`topask)!(((';max);(flip;(enlist,bc)));((';min);(flip;(enlist,ac))))];
/####################################
\
/To add
/Add bid and ask src
/Add id of the quote at top of book
/Add time of quote at top of book
/Can this be more efficient
/Add logic to time out stale quotes via a "valid" flag
/Set a timer to simulate a feed

