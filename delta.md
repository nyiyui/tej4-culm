# Delta value

```
let a = light_normalized[0] left
let b = light_normalized[1] centre
let c = light_normalized[2] right

delta_v1 = b - a

delta_v2:
case 1 (leftmost) = -2
   -a--b--c-
 |
case 2 (left) = -1
   -a--b--c-
    |
case 3 (centre) = 0
   -a--b--c
       |
```
