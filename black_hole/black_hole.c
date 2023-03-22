//
//  Copyright (c) 2007. Màrius Montón. CEPHIS-UAB
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>


/*  command  */
#define OP_SET 1
#define GO     5

#define START  1
#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4


int main(int argc, char* argv[])
{

    int i = 0;
    int fin = 0;
    fin = open("/dev/bh", O_RDWR);

    for(i = 0; i < 20; i++)
    {
        write(fin, &i, 4);
    }

    close(fin);


    /*
    int fin=0;

    int operand[2];
    int temp[4];

    fin = open("/dev/calculator", O_RDWR);

    if(fin==0) {
        perror("Error opening device\n");
        exit(1);
    }

    if(argc != 3){
        printf("invalid input!!!\n");
        exit(1);
    }

    operand[0] = atoi(argv[1]);
    operand[1] = atoi(argv[2]);

    write(fin, operand, 8);

    ioctl(fin, OP_SET, ADD);
    ioctl(fin, GO, START);
    read(fin, temp, 4);

    ioctl(fin, OP_SET, SUB);
    ioctl(fin, GO, START);
    read(fin, temp+1, 4);

    ioctl(fin, OP_SET, MUL);
    ioctl(fin, GO, START);
    read(fin, temp+2, 4);

    ioctl(fin, OP_SET, DIV);
    ioctl(fin, GO, START);
    read(fin, temp+3, 4);

    printf("%d + %d = %d\n", operand[0], operand[1], temp[0]);
    printf("%d - %d = %d\n", operand[0], operand[1], temp[1]);
    printf("%d * %d = %d\n", operand[0], operand[1], temp[2]);
    printf("%d / %d = %d\n", operand[0], operand[1], temp[3]);

    close(fin);
    */

    return 0;
}

