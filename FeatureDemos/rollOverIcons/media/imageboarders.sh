#!/bin/bash
convert ./1.jpg -bordercolor black -border 20 -bordercolor white -border 1 2.jpg


convert -border 2x2 old.jpg new.jpg

convert -border 2x2 -bordercolor "#FF0000" old.jpg new.jpg
