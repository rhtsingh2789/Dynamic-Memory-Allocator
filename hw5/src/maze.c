void maze_init(char **template){
}

void maze_fini(){}
int maze_get_rows(){}
int maze_get_cols(){}
int maze_set_player(OBJECT avatar, int row, int col){}
int maze_set_player_random(OBJECT avatar, int *rowp, int *colp){}
void maze_remove_player(OBJECT avatar, int row, int col){}
int maze_move(int row, int col, int dir){}
OBJECT maze_find_target(int row, int col, DIRECTION dir){}
int maze_get_view(VIEW *view, int row, int col, DIRECTION gaze, int depth){}
void show_view(VIEW *view, int depth){}
