<article>
<h2>--- Day 15: Rambunctious Recitation ---</h2>

You catch the airport shuttle and try to book a new flight to your vacation island. Due to the storm, all direct flights have been cancelled, but a route is available to get around the storm. You take it.

While you wait for your flight, you decide to check in with the Elves back at the North Pole. They're playing a <strong>memory game</strong> and are ever so excited to explain the rules!

In this game, the players take turns saying <strong>numbers</strong>. They begin by taking turns reading from a list of <strong>starting numbers</strong> (your puzzle input). Then, each turn consists of considering the <strong>most recently spoken number</strong>:

* If that was the <strong>first</strong> time the number has been spoken, the current player says <strong>`0`</strong>.
* Otherwise, the number had been spoken before; the current player announces <strong>how many turns apart</strong> the number is from when it was previously spoken.

So, after the starting numbers, each turn results in that player speaking aloud either <strong>`0`</strong> (if the last number is new) or an <strong>age</strong> (if the last number is a repeat).

For example, suppose the starting numbers are `0,3,6`:

* <strong>Turn 1</strong>: The `1`st number spoken is a starting number, <strong>`0`</strong>.
* <strong>Turn 2</strong>: The `2`nd number spoken is a starting number, <strong>`3`</strong>.
* <strong>Turn 3</strong>: The `3`rd number spoken is a starting number, <strong>`6`</strong>.
* <strong>Turn 4</strong>: Now, consider the last number spoken, `6`. Since that was the first time the number had been spoken, the `4`th number spoken is <strong>`0`</strong>.
* <strong>Turn 5</strong>: Next, again consider the last number spoken, `0`. Since it <strong>had</strong> been spoken before, the next number to speak is the difference between the turn number when it was last spoken (the previous turn, `4`) and the turn number of the time it was most recently spoken before then (turn `1`). Thus, the `5`th number spoken is `4 - 1`, <strong>`3`</strong>.
* <strong>Turn 6</strong>: The last number spoken, `3` had also been spoken before, most recently on turns `5` and `2`. So, the `6`th number spoken is `5 - 2`, <strong>`3`</strong>.
* <strong>Turn 7</strong>: Since `3` was just spoken twice in a row, and the last two turns are `1` turn apart, the `7`th number spoken is <strong>`1`</strong>.
* <strong>Turn 8</strong>: Since `1` is new, the `8`th number spoken is <strong>`0`</strong>.
* <strong>Turn 9</strong>: `0` was last spoken on turns `8` and `4`, so the `9`th number spoken is the difference between them, <strong>`4`</strong>.
* <strong>Turn 10</strong>: `4` is new, so the `10`th number spoken is <strong>`0`</strong>.

(The game ends when the Elves get sick of playing or dinner is ready, whichever comes first.)

Their question for you is: what will be the <strong>`2020`th</strong> number spoken? In the example above, the `2020`th number spoken will be `436`.

Here are a few more examples:

* Given the starting numbers `1,3,2`, the `2020`th number spoken is <strong>`1`</strong>.
* Given the starting numbers `2,1,3`, the `2020`th number spoken is <strong>`10`</strong>.
* Given the starting numbers `1,2,3`, the `2020`th number spoken is <strong>`27`</strong>.
* Given the starting numbers `2,3,1`, the `2020`th number spoken is <strong>`78`</strong>.
* Given the starting numbers `3,2,1`, the `2020`th number spoken is <strong>`438`</strong>.
* Given the starting numbers `3,1,2`, the `2020`th number spoken is <strong>`1836`</strong>.

Given your starting numbers, <strong>what will be the `2020`th number spoken?</strong>
</article>

<article>
<h2>--- Part Two ---</h2>

Impressed, the Elves issue you a challenge: determine the `30000000`th number spoken. For example, given the same starting numbers as above:

* Given `0,3,6`, the `30000000`th number spoken is <strong>`175594`</strong>.
* Given `1,3,2`, the `30000000`th number spoken is <strong>`2578`</strong>.
* Given `2,1,3`, the `30000000`th number spoken is <strong>`3544142`</strong>.
* Given `1,2,3`, the `30000000`th number spoken is <strong>`261214`</strong>.
* Given `2,3,1`, the `30000000`th number spoken is <strong>`6895259`</strong>.
* Given `3,2,1`, the `30000000`th number spoken is <strong>`18`</strong>.
* Given `3,1,2`, the `30000000`th number spoken is <strong>`362`</strong>.

Given your starting numbers, <strong>what will be the `30000000`th number spoken?</strong>
</article>
