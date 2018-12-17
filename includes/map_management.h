#ifndef _MAP_MANAGEMENT_H_
#define _MAP_MANAGEMENT_H_

int	**get_array_map();
void	add_destructible_elements(t_game_info *game_info, int **map_pointer);
void	manage_bombs(t_game_info *game_info, int **map_pointer);
int	is_there_a_wall(t_game_info *game_info, int **map_pointer, int x, int y);

#endif /* !_MAP_MANAGEMENT_H_ */
