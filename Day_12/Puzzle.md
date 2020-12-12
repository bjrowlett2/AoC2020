<article>
<h2>--- Day 12: Rain Risk ---</h2>

Your ferry made decent progress toward the island, but the storm came in faster than anyone expected. The ferry needs to take <strong>evasive actions</strong>!

Unfortunately, the ship's navigation computer seems to be malfunctioning; rather than giving a route directly to safety, it produced extremely circuitous instructions. When the captain uses the <a href="https://en.wikipedia.org/wiki/Public_address_system">PA system</a> to ask if anyone can help, you quickly volunteer.

The navigation instructions (your puzzle input) consists of a sequence of single-character <strong>actions</strong> paired with integer input <strong>values</strong>. After staring at them for a few minutes, you work out what they probably mean:

* Action <strong>`N`</strong> means to move <strong>north</strong> by the given value.
* Action <strong>`S`</strong> means to move <strong>south</strong> by the given value.
* Action <strong>`E`</strong> means to move <strong>east</strong> by the given value.
* Action <strong>`W`</strong> means to move <strong>west</strong> by the given value.
* Action <strong>`L`</strong> means to turn <strong>left</strong> the given number of degrees.
* Action <strong>`R`</strong> means to turn <strong>right</strong> the given number of degrees.
* Action <strong>`F`</strong> means to move <strong>forward</strong> by the given value in the direction the ship is currently facing.

The ship starts by facing <strong>east</strong>. Only the `L` and `R` actions change the direction the ship is facing. (That is, if the ship is facing east and the next instruction is `N10`, the ship would move north 10 units, but would still move east if the following action were `F`.)

For example:

```
F10
N3
F7
R90
F11
```

These instructions would be handled as follows:

* `F10` would move the ship 10 units east (because the ship starts by facing east) to <strong>east 10, north 0</strong>.
* `N3` would move the ship 3 units north to <strong>east 10, north 3</strong>.
* `F7` would move the ship another 7 units east (because the ship is still facing east) to <strong>east 17, north 3</strong>.
* `R90` would cause the ship to turn right by 90 degrees and face <strong>south</strong>; it remains at <strong>east 17, north 3</strong>.
* `F11` would move the ship 11 units south to <strong>east 17, south 8</strong>.

At the end of these instructions, the ship's <a href="https://en.wikipedia.org/wiki/Manhattan_distance">Manhattan distance</a> (sum of the absolute values of its east/west position and its north/south position) from its starting position is `17 + 8` = <strong>`25`</strong>.

Figure out where the navigation instructions lead. <strong>What is the Manhattan distance between that location and the ship's starting position?</strong>
</article>

<article>
<h2>--- Part Two ---</h2>

Before you can give the destination to the captain, you realize that the actual action meanings were printed on the back of the instructions the whole time.

Almost all of the actions indicate how to move a <strong>waypoint</strong> which is relative to the ship's position:

* Action <strong>`N`</strong> means to move the waypoint <strong>north</strong> by the given value.
* Action <strong>`S`</strong> means to move the waypoint <strong>south</strong> by the given value.
* Action <strong>`E`</strong> means to move the waypoint <strong>east</strong> by the given value.
* Action <strong>`W`</strong> means to move the waypoint <strong>west</strong> by the given value.
* Action <strong>`L`</strong> means to rotate the waypoint around the ship <strong>left</strong> (<strong>counter-clockwise</strong>) the given number of degrees.
* Action <strong>`R`</strong> means to rotate the waypoint around the ship <strong>right</strong> (<strong>clockwise</strong>) the given number of degrees.
* Action <strong>`F`</strong> means to move <strong>forward</strong> to the waypoint a number of times equal to the given value.

The waypoint starts <strong>10 units east and 1 unit north</strong> relative to the ship. The waypoint is relative to the ship; that is, if the ship moves, the waypoint moves with it.

For example, using the same instructions as above:

* `F10` moves the ship to the waypoint 10 times (a total of <strong>100 units east and 10 units north</strong>), leaving the ship at <strong>east 100, north 10</strong>. The waypoint stays 10 units east and 1 unit north of the ship.
* `N3` moves the waypoint 3 units north to <strong>10 units east and 4 units north of the ship</strong>. The ship remains at <strong>east 100, north 10</strong>.
* `F7` moves the ship to the waypoint 7 times (a total of <strong>70 units east and 28 units north</strong>), leaving the ship at <strong>east 170, north 38</strong>. The waypoint stays 10 units east and 4 units north of the ship.
* `R90` rotates the waypoint around the ship clockwise 90 degrees, moving it to <strong>4 units east and 10 units south of the ship</strong>. The ship remains at <strong>east 170, north 38</strong>.
* `F11` moves the ship to the waypoint 11 times (a total of <strong>44 units east and 110 units south</strong>), leaving the ship at <strong>east 214, south 72</strong>. The waypoint stays 4 units east and 10 units south of the ship.

After these operations, the ship's Manhattan distance from its starting position is `214 + 72` = <strong>`286`</strong>.

Figure out where the navigation instructions actually lead. <strong>What is the Manhattan distance between that location and the ship's starting position?</strong>
</article>
