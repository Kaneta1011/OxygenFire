count 9
index 0 {
 name	CANDLE_B
 type	6
 checkOn	END@
 checkOff	RESETCANDLE END@
 pos	-1.4009 0 1.85466
 angle	0 0 0
 scale	1.46838 1.46838 1.46838
}
index 1 {
 name	GOAL
 type	11
 checkOn	RESETCANDLE_CHECKER END@
 checkOff	END@
 pos	-0 0 -4.27241
 angle	0 0 0
 scale	0.756403 0.756403 0.756403
}
index 2 {
 name	RESETCANDLE_CHECKER
 type	13
 checkOn	RESETCANDLE CANDLE_B CANDLE_R CANDLE_Y CANDLE_G CANDLE_W CANDLE_P END@
 checkOff	END@
 correctAnswer	BYWRPG
}
index 3 {
 name	RESETCANDLE
 type	12
 checkOn	END@
 checkOff	END@
 pos	0.021343 0 0.025095
 angle	0 0 0
 scale	2.18839 2.18839 2.18839
}
index 4 {
 name	CANDLE_R
 type	6
 checkOn	END@
 checkOff	RESETCANDLE END@
 pos	-2.41911 0 -0.000741
 angle	-0 0 0
 scale	1.46838 1.46838 1.46838
}
index 5 {
 name	CANDLE_Y
 type	6
 checkOn	END@
 checkOff	RESETCANDLE END@
 pos	-1.41479 0 -1.92403
 angle	-0 0 0
 scale	1.46838 1.46838 1.46838
}
index 6 {
 name	CANDLE_G
 type	6
 checkOn	END@
 checkOff	RESETCANDLE END@
 pos	1.46307 0 -1.92394
 angle	-0 0 0
 scale	1.46838 1.46838 1.46838
}
index 7 {
 name	CANDLE_W
 type	6
 checkOn	END@
 checkOff	RESETCANDLE END@
 pos	2.39206 0 0.010265
 angle	-0 0 0
 scale	1.46838 1.46838 1.46838
}
index 8 {
 name	CANDLE_P
 type	6
 checkOn	END@
 checkOff	RESETCANDLE END@
 pos	1.41964 0 1.85466
 angle	-0 0 0
 scale	1.46838 1.46838 1.46838
}
