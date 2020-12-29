R 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 G 0
0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0

1. check if the enemy bomb{
    protect{
        if influence{
            eat
            eat after eatten
        }
    }
}
2. check if anyone need to be arounded{
    is bomb-1 and havent be around
    (1)corner
    (2)edge
    (3)other
}
3. if corner are empty{
    corner
}
4. check if anyone is around{
    if not bomb-1{
        +1
    }
}
5. if exceed limit{
    (1) check if any enemy can be eatten
    (2) check to new corner
}
else{
    if anyone can around other{
        around the one need less number of chess first!
        (1) edge
        (2) other
        change the enemy to mine{
            around it
            eat(better not to inflence other)
        }
    }
}
