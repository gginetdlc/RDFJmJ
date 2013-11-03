/*************************************************************************/
/*  struct.c                                                             */
/*  author: Jose Miguel Noblecilla                                       */
/*  created: 01/11/2013                                                  */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*  This program is free software: you can redistribute it and/or modify */
/*  it under the terms of the GNU General Public License as published by */
/*  the Free Software Foundation, either version 3 of the License, or    */
/*  (at your option) any later version.                                  */
/*                                                                       */
/*  This program is distributed in the hope that it will be useful,      */ 
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/*  GNU General Public License for more details.                         */
/*                                                                       */
/*  You should have received a copy of the GNU General Public License    */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/> */
/*************************************************************************/


#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "struct_support.h"

int main(int argc, char *argv[]){

	if ( argc == 2 ){
		time_t start, stop;
	    clock_t ticks; 
		time(&start);
		FILE *log = init_log(argv[1]);
		printf("%s : Log activated\n", curdate());
		generate_triplets(log, argv[1]);
		if ( !fork() ){
			if ( !fork() ){
				get_column( init_log("g-col1"), 1, 0 );
			}else{
				get_column( init_log("g-col2"), 2, 0 ); // this is the lighter one!! 
				// I'm gonna re-use this process to generate the hash of the triplets
				insertTrip();
			}
			return 0;
		}else{
			if ( !fork()) {
				get_column( init_log("g-col3"), 3, 1 );
				return 0;
			}
			while (1) {
				int status;
				pid_t done = wait(&status);
				if (done == -1) {
					if (errno == ECHILD) break; // no more child processes
				} else {
					if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
						exit(1);
					}
				}
			}
			time(&stop);
			printf("Finished in about %.3f seconds. \n\n", difftime(stop, start));
			fprintf(log, "Finished in about %.3f seconds. \n\n", difftime(stop, start));
		}
		fclose(log);
	}else{
		puts("USAGE: ./run rdf-file.ttl");

	}

	return 0;
}
