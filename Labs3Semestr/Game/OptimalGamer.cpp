#include "Player.h"

//Специфика умного выбора
Type_player OptimalGamer::return_type_player() {
    return optimal_player;
}

char OptimalGamer::Define_name_column_by_num(int &num) {
    switch (num)
    {
        case 0:
            return 'A';
        case 1:
            return 'B';
        case 2:
            return 'C';
        case 3:
            return 'D';
        case 4:
            return 'E';
        case 5:
            return 'F';
        case 6:
            return 'G';
        case 7:
            return 'H';
        case 8:
            return 'I';
        case 9:
            return 'J';
    }
}

void OptimalGamer::Add_ships(int number_of_ships) {

    /*Изначально умный ИИ расстановит большие корабли по краям поля, чтобы получить максимальное число свободных
      клеток, для расстановки маленьких кораблей*/
    const std::vector<int> big_ship_size = {4, 3, 3, 2, 2, 2};
    std::cout << "Create optimal ships location..." << std::endl;
    for(unsigned long i = 0; i < big_ship_size.size(); i++)
    {
        shipDirection = 'V';//Все большие корабли расположим вертикально
        int size = big_ship_size[i];
        if(size == 4)//Устанавливаем линкор
        {
            shipColumn = 'A';
            shipRow = 3;

            Ship tmp_ship(size);
            for(int j = 1; j <= size; j++)
            {
                gameBoard[shipRow - j + 1][ConvertColumn(shipColumn)] = 'S';
                tmp_ship.Add_coordinate(shipRow - j + 1, ConvertColumn(shipColumn));
            }
            player_ships.push_back(tmp_ship);
            ship_size[size - 1]--;
        }
        else if(size == 3)//Устанавливаем крейсеры
        {
            shipColumn = 'J';
            if(ship_size[size - 1] == 2)
            {
                shipRow = 2;
                Ship tmp_ship(size);
                for(int j = 1; j <= size; j++)
                {
                    gameBoard[shipRow - j + 1][ConvertColumn(shipColumn)] = 'S';
                    tmp_ship.Add_coordinate(shipRow - j + 1, ConvertColumn(shipColumn));
                }
                player_ships.push_back(tmp_ship);
                ship_size[size - 1]--;
            }
            else if(ship_size[size - 1] == 1)
            {
                shipRow = 9;
                Ship tmp_ship(size);
                for(int j = 1; j <= size; j++)
                {
                    gameBoard[shipRow - j + 1][ConvertColumn(shipColumn)] = 'S';
                    tmp_ship.Add_coordinate(shipRow - j + 1, ConvertColumn(shipColumn));
                }
                player_ships.push_back(tmp_ship);
                ship_size[size - 1]--;
            }
        }
        else if(size == 2)//Устанавливаем эсминцы
        {
            if(ship_size[size - 1] > 1)
            {
                shipColumn = 'A';
                if(ship_size[size - 1] == 3)
                {
                    shipRow = 6;
                    Ship tmp_ship(size);
                    for(int j = 1; j <= size; j++)
                    {
                        gameBoard[shipRow - j + 1][ConvertColumn(shipColumn)] = 'S';
                        tmp_ship.Add_coordinate(shipRow - j + 1, ConvertColumn(shipColumn));
                    }
                    player_ships.push_back(tmp_ship);
                    ship_size[size - 1]--;
                }
                else if(ship_size[size - 1] == 2)
                {
                    shipRow = 9;
                    Ship tmp_ship(size);
                    for(int j = 1; j <= size; j++)
                    {
                        gameBoard[shipRow - j + 1][ConvertColumn(shipColumn)] = 'S';
                        tmp_ship.Add_coordinate(shipRow - j + 1, ConvertColumn(shipColumn));
                    }
                    player_ships.push_back(tmp_ship);
                    ship_size[size - 1]--;
                }
            }
            else if(ship_size[size - 1] == 1)
            {
                shipColumn = 'J';
                shipRow = 5;
                Ship tmp_ship(size);
                for(int j = 1; j <= size; j++)
                {
                    gameBoard[shipRow - j + 1][ConvertColumn(shipColumn)] = 'S';
                    tmp_ship.Add_coordinate(shipRow - j + 1, ConvertColumn(shipColumn));
                }
                player_ships.push_back(tmp_ship);
                ship_size[size - 1]--;
            }
        }
    }
    //Устанавливаем случайным образом одноклеточные корабли 4 шт.
    for(int i = 0; i < 4; i++)
    {
        bool correct_input = false;
        while (!correct_input)
        {
            int size = 1;
            ship_size[size - 1]--;
            shipDirection = 'H';//Для одноклеточных кораблей направление не имеет значения
            int Column = rand()%10;
            shipColumn = Define_name_column_by_num(Column);
            shipRow = rand()%10;

            if(ValidCoordinate(shipRow, shipColumn, shipDirection, size))
            {
                correct_input = true;
                Ship tmp_ship(size);//Создаем наш корабль, чтобы поместить его на карту
                for(int j = 1; j <= size; j++)
                {
                    gameBoard[shipRow][(ConvertColumn(shipColumn))+ j - 1] = 'S';//S - палуба корабля
                    tmp_ship.Add_coordinate(shipRow, (ConvertColumn(shipColumn)) + j -1);
                }
                player_ships.push_back(tmp_ship);
            }
            else
            {
                ship_size[size - 1]++;
            }
        }
    }
    PrintBoard();
}

void OptimalGamer::Choose_coordinates_for_attack(int &coord_row, char &coord_column) {
    coord_row = rand()%10;
    int Column_num = rand()%10;
    coord_column = Define_name_column_by_num(Column_num);
}

void OptimalGamer::Get_Fire(int row, char column, Gamer* enemy) {
    bool retry = true;//Повторная попытка если пользователь ввел координаты, которые уже проверял ранее
    while (retry)
    {
        char PotentialRetry = Find_on_attack_board(row, column);//Ищем введенные координаты на поле противника
        if(PotentialRetry == '#' || (PotentialRetry == 'O' && enemy->return_type_player() == optimal_player))
        {
            //Если в данной координате уже был допущен промах или это клетка окуржения мретвого судна
            enemy->Choose_coordinates_for_attack(row, column);
        }
        else if(PotentialRetry == 'H' || PotentialRetry == 'X')
        {
            //Если в данной позиции уже был нанесен удар или в ней обломки убитого корабля
            enemy->Choose_coordinates_for_attack(row, column);
        }
        else
            retry = false;
    }

    char location = Find(row, column);//Проверяем на карте с кораблями, куда попал удар
    if(location == '*')
    {
        //Пустая клетка
        std::cout << "CPU(Optimal) missed at position:[" << column << "][" << row << "]\n";
        attackBoard[row][ConvertColumn(column)] = '#';//M - missed промах
        return;
    }
    else if(location == 'S')
    {
        //Палуба корабля
        //Проверяем в какой из кораблей мы попали
        for(unsigned long i  = 0; i < player_ships.size(); i++)
        {
            if(player_ships[i].Check_for_hit(row, ConvertColumn(column)))
            {
                std::cout << "CPU(Optimal) have hit at location:[" << column << "][" << row << "]\n";
                attackBoard[row][ConvertColumn(column)] = 'H';//H - hit нанесен урон палубе
                gameBoard[row][ConvertColumn(column)] = 'H';
                //Если это была последняя палуба корабля
                if(player_ships[i].Get_length() == 0)
                {
                    count_ships--;
                    std::cout << "CPU(Optimal) sunk a ship!\n";
                    std::vector<std::pair<int, int>> ship_coords = player_ships[i].ReturnCoordinates();
                    for(unsigned long j = 0; j < ship_coords.size(); j++)
                    {
                        gameBoard[ship_coords[j].first][ship_coords[j].second] = 'X';
                        attackBoard[ship_coords[j].first][ship_coords[j].second] = 'X';

                        if(enemy->return_type_player() == optimal_player)
                        {
                            //Клетки окружения уничтоженного корабля рассматриваться не будут в дальнейшем (если противник наносивший удар умный ИИ)
                            if(ship_coords[j].first - 1 >= 0 && attackBoard[ship_coords[j].first - 1][ship_coords[j].second] != 'X')
                            {
                                attackBoard[ship_coords[j].first - 1][ship_coords[j].second] = 'O';

                                if(ship_coords[j].second + 1 <= 9 && attackBoard[ship_coords[j].first - 1][ship_coords[j].second + 1] != 'X')
                                    attackBoard[ship_coords[j].first - 1][ship_coords[j].second + 1] = 'O';

                                if(ship_coords[j].second - 1 >= 0 && attackBoard[ship_coords[j].first - 1][ship_coords[j].second - 1] != 'X')
                                    attackBoard[ship_coords[j].first - 1][ship_coords[j].second - 1] = 'O';
                            }

                            if(ship_coords[j].first + 1 <= 9 && attackBoard[ship_coords[j].first + 1][ship_coords[j].second] != 'X')
                            {
                                attackBoard[ship_coords[j].first + 1][ship_coords[j].second] = 'O';

                                if(ship_coords[j].second + 1 <= 9 && attackBoard[ship_coords[j].first][ship_coords[j].second + 1] != 'X')
                                    attackBoard[ship_coords[j].first + 1][ship_coords[j].second + 1] = 'O';

                                if(ship_coords[j].second - 1 >= 0 && attackBoard[ship_coords[j].first][ship_coords[j].second - 1] != 'X')
                                    attackBoard[ship_coords[j].first + 1][ship_coords[j].second - 1] = 'O';
                            }

                            if(ship_coords[j].second + 1 <= 9 && attackBoard[ship_coords[j].first][ship_coords[j].second + 1] != 'X')
                                attackBoard[ship_coords[j].first][ship_coords[j].second + 1] = 'O';

                            if(ship_coords[j].second - 1 >= 0 && attackBoard[ship_coords[j].first][ship_coords[j].second - 1] != 'X')
                                attackBoard[ship_coords[j].first][ship_coords[j].second - 1] = 'O';
                        }
                    }
                }
            }
        }
    }

}