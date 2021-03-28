# Chain Reaction Algorithm
The goal of this project is to create a algorithm which have a maximum ratio to beat the enemy in the chain reaction game. :wind_chime: 

### Game description
for the game rule, plz attach to [**here**](https://www.youtube.com/watch?v=7YGgI02LOtw).
![](https://i.imgur.com/op1l6zh.png)

### Algorithm
* Strategy
    * 鋪路
    按照「角落>四邊>中間」的順序來下棋，意即，角落被佔滿了 (差一個就被引爆)則找四邊的棋子下，若四邊的棋子被下滿了，則找中 間的下。利用沒辦法被引爆的棋子把差一個就被引爆的棋子圍起來(有需 要的時候再引爆)。
    * 防守
    每一次下棋前先檢查即將被引爆的對手是否會讓我方的數量減少， 若會減少則將被引爆的對手收集起來，按照傷害的程度排列，一個一個檢 查能不能將該引爆點變成我方。若每一顆皆無法，則採用攻擊手段。
    * 攻擊
    收集我方即將被引爆的點，一個一個比較將讓對手減少最多數量的 棋子引爆。
    
* Main flow chart
![](https://i.imgur.com/WFg5Ucd.png)
    * 檢查棋盤是否為空，若為空，則可按照優先順序下棋(先下角落，再四邊， 接著中間)。回傳點的位置。
    * 若棋盤不為空，檢查敵對是否有將被引爆的點，若有，則將所有敵對會被 引爆的點收集起來，比較傷害程度(計算引爆之後，我方所剩棋子的數量 比較)，由傷害程度高到低一一檢查是否有可以防守的對象。回傳點的位 置。
    * 若無可防守的對象，則檢查是否可採取攻擊，檢查我方是否有可以被引爆 的點，若有，找出引爆後可以減少最多敵軍的點。回傳點的位置。
    * 若無，則繼續鋪路，檢查我方是否有即將被引爆的點，卻沒有被其他點圍 起來的(不圍起來，可能會幫敵軍鋪路)。若有，則回傳點的位置。
    * 若無，則按照優先順續，下棋(角落>四邊>中間)在棋子不差一點被引
爆的情況下(數量須少於，滿- 1)，增加其數量。

### Function

* `check_if_the_enemy_burst`
> 檢查是否有敵人要被引爆，若有，將該點加入名為`EnemyBurst`之一維陣列中，最後將陣列中所有的模擬每一點引爆後 的狀況，選擇我方所剩最少的情況去防守或攻擊。
![](https://i.imgur.com/QB1J8KK.png)

* `protect_and_attack`
> 若無法防守，及攻擊，若無法攻擊，則近道下一階段 ，繼續鋪路。
![](https://i.imgur.com/awW1VAa.png)

* `check_if_any_need_around`
> 檢查是否有我方之棋子即將被引爆，卻沒有被 包圍(保護)的。
* `priorityadd`
> 按照優先順序找可以下的棋子，先利用`check_corner`檢查角 落是否有位置，若有則回傳，若無則利用`check_adge`檢查四邊是否有位 置，若有則回傳，若無則利用`check_center`檢查是否有位置，有則回傳， 無則利用random move的方式下棋，並回傳(不太會走到這一步，只是以防萬一)。
![](https://i.imgur.com/5yYIslR.png)




