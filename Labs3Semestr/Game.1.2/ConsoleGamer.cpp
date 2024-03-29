#include "Player.h"

//Методы, содержащие особенности консольного игрока
Type_player ConsoleGamer::return_type_player() {
    return console_player;
}

void ConsoleGamer::Row_coordinate(int &check_first_coordinate) {
    std::cin >> check_first_coordinate;

    while (std::cin.fail() || check_first_coordinate < 0 || check_first_coordinate > 9)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERROR: PLEASE ENTER A VALUE (0 - 9):";
        std::cin >> check_first_coordinate;
    }
}

void ConsoleGamer::Column_coordinate(char &check_second_coordinate) {
    std::cin >> check_second_coordinate;
    check_second_coordinate = (char)toupper(check_second_coordinate);
    int tmp = int(check_second_coordinate);

    while (tmp < 65 || tmp > 74 || std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "ERROR: PLEASE ENTER A COLUMN CHARACTER (A-J):";
        std::cin >> check_second_coordinate;
        check_second_coordinate = (char)toupper(check_second_coordinate);
        tmp = int(check_second_coordinate);
    }
}

void ConsoleGamer::PrintBoard() {
    std::cout << "\t\t<YOUR BOARD>" << std::endl;
    std::cout << "   [A][B][C][D][E][F][G][H][I][J]\n";

    for(int x = 0; x < ROWS; x++)
    {
        std::cout << "[" <<  x << "]";
        for(int y = 0; y < COLUMNS; y++)
        {
            std::cout << " " << gameBoard[x][y] << " ";
        }
        std::cout << std::endl;
    }
}

void ConsoleGamer::Add_ships(int number_of_ships) {
    std::cout << "\tYou should set 10 ships on your map" << std::endl;
    std::cout << "1 battleship, size: 4" << std::endl;
    std::cout << "2 cruisers, size: 3" << std::endl;
    std::cout << "3 destroyers, size: 2" << std::endl;
    std::cout << "4 torpedo boats, size 1\n" << std::endl;
    std::cout << "REMEMBER! YOU CANNOT PUT SHIPS NEAR EACH OTHER!" << std::endl;
    std::cout << "SHIPS SHOULD BE AT THE DISTANCE AT LEAST IN THE ONE MESH!\n" << std::endl;

    PrintBoard();//На начальный момент поле пользователя пустое

    char is_rand = 'n';
    std::cout << "Do you want to create random ship location automatically?(y - yes/ n - no):";
    std::cin >> is_rand;

    while (std::cin.fail() || (is_rand != 'y' && is_rand != 'n'))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Do you want to create random ship location automatically?(y - yes/ n - no):";
        std::cin >> is_rand;
    }

    int size = 0;
    char shipDirection = '-';
    int shipRow = 0;
    char shipColumn;

    if(is_rand == 'y')
    {
        std::cout << "Create random ships location..." << std::endl;
        for(int i = 0; i < number_of_ships; i++)
        {
            bool correct_input = false;
            while (!correct_input)
            {
                //Задаем рамер добавляемого корабля
                while(true) {
                    size = rand()%4 + 1;
                    if (size > 0 && size <= 4) {
                        if (ship_size[size - 1] > 0){
                            ship_size[size - 1]--;
                            break;
                        }
                    }
                }

                //Определяем в каком положени располагается корабль
                int dir = rand()%2 + 1;
                if(dir == 1)
                    shipDirection = 'H';
                else if(dir == 2)
                    shipDirection = 'V';


                if(shipDirection == 'H')
                {
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
                else if(shipDirection == 'V')
                {
                    int Column = rand()%10;
                    shipColumn = Define_name_column_by_num(Column);
                    shipRow = rand()%10;

                    if(ValidCoordinate(shipRow, shipColumn, shipDirection, size))
                    {
                        correct_input = true;
                        Ship tmp_ship(size);
                        for(int j = 1; j <= size; j++)
                        {
                            gameBoard[shipRow - j + 1][ConvertColumn(shipColumn)] = 'S';
                            tmp_ship.Add_coordinate(shipRow - j + 1, ConvertColumn(shipColumn));
                        }
                        player_ships.push_back(tmp_ship);
                    }
                    else
                    {
                        ship_size[size - 1]++;
                    }
                }
                else{
                    ship_size[size - 1]++;
                }
            }
        }
    }
    else
    for(int i = 0; i < number_of_ships; i++)
    {
        //Спрашиваем в каком направлении расположен корабль(веритикально/горизонтально)
        bool correct_input = false;//Если параметр станет true то корабль размещен корректно
        while (!correct_input)
        {
            std::cout << "Choose one of the unidentified ships, indicating its size.\n";
            std::cout << "Available ships:" << std::endl;

            for(unsigned long s = 0; s < ship_size.size(); s++)
            {
                std::cout << "Ship count (size:" << s + 1 << "): " << ship_size[s] << std::endl;
            }

            //Задаем рамер добавляемого корабля
            while(true) {
                char curr_size;
                std::cout << "Print size of the ship:";
                std::cin >> curr_size;
                if (!isdigit(curr_size)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "WRONG INPUT FOR SIZE, PLEASE PRINT CORRECT DIGIT.\n";
                }
                else {
                    size = std::stoi(&curr_size);
                    if (size < 0 || size > 4) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "WRONG INPUT FOR SIZE, PLEASE PRINT CORRECT DIGIT.\n";
                    }
                    else if (ship_size[size - 1] <= 0) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "YOU ALREADY USED ALL SHIPS OF THIS TYPE!" << std::endl;
                    }
                    else {
                        ship_size[size - 1]--;
                        break;
                    }
                }
            }

            //Определяем в каком положени располагается корабль
            std::cout << "Which direction would you like to place ship? (H - horizontal, V - vertical):";
            std::cin >> shipDirection;
            shipDirection = (char)toupper(shipDirection);


            //Спрашиваем у пользователя, где находится самая левая координата(в случае если горизонтальное расположение)
            //Справишваем у пользователя, где находится самая нижняя координата(в случае вериткального положения)
            if(shipDirection == 'H')
            {
                std::cout << "What is the left-most column position you would like your ship to be placed?(A-J):";
                Column_coordinate(shipColumn);
                std::cout << "What is the left-most row position you would like your ship to be placed?(0-9):";
                Row_coordinate(shipRow);

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
                    system("clear");
                    std::cout << "\nSORRY, INVALID COORDINATE.TRY AGAIN!\n";
                    PrintBoard();
                }
            }
            else if(shipDirection == 'V')
            {
                std::cout << "What is the bottom-most column position you would like your ship to be placed?(A-J):";
                Column_coordinate(shipColumn);
                std::cout << "What is the bottom-most row position you would like your ship to be placed?(0-9):";
                Row_coordinate(shipRow);

                if(ValidCoordinate(shipRow, shipColumn, shipDirection, size))
                {
                    correct_input = true;
                    Ship tmp_ship(size);
                    for(int j = 1; j <= size; j++)
                    {
                        gameBoard[shipRow - j + 1][ConvertColumn(shipColumn)] = 'S';
                        tmp_ship.Add_coordinate(shipRow - j + 1, ConvertColumn(shipColumn));
                    }
                    player_ships.push_back(tmp_ship);
                }
                else
                {
                    ship_size[size - 1]++;
                    system("clear");
                    std::cout << "\nSORRY, INVALID COORDINATE.TRY AGAIN!\n";
                    PrintBoard();
                }
            }
            else{
                ship_size[size - 1]++;
                system("clear");
                std::cout << "\nERROR! NOT A VALID SHIP DIRECTION\n";
                PrintBoard();
            }
        }
        system("clear");
        PrintBoard();
    }
}

//Ход игрока, за которого играет пользователь
void ConsoleGamer::Choose_coordinates_for_attack(int &coord_row, char &coord_column) {
    std::cout << "Which column will you fire at?(A-J):";
    Column_coordinate(coord_column);
    std::cout << "Which row you will fire at?(0-9):";
    Row_coordinate(coord_row);
    bool retry = true;
    while (retry)
    {
        if(attackBoard[coord_row][ConvertColumn(coord_column)] == '#' ||
           attackBoard[coord_row][ConvertColumn(coord_column)] == 'X' ||
           attackBoard[coord_row][ConvertColumn(coord_column)] == 'H')
        {
            std::cout << "YOU HAVE ALREADY PRINT THIS COORDINATES! PLEASE PRINT ANOTHER!\n";
            std::cout << "Which column will you fire at?(A-J):";
            Column_coordinate(coord_column);
            std::cout << "Which row you will fire at?(0-9):";
            Row_coordinate(coord_row);
        }
        else
            retry = false;
    }
}




