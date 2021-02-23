#include "utils.h"
#include "parser.h"
#include <stdlib.h>

/****************************** M A C R O S ******************************/

#define SIZE_POINTER 8
#define NUMBER_OF_PORTS 6
#define NUMBER_OF_SWITCHS 8
#define NUMBER_OF_LISTS 100



void main()
{
    int count_switches = 0;
    int* ptr_count_switches = &count_switches;

    switx **ptr_switches;
    ptr_switches = malloc(sizeof(switx)*NUMBER_OF_SWITCHS);

    ReadConfigFile(ptr_count_switches, ptr_switches);
    printf("CountSwitches: %u\n", count_switches);

    for(int i=0; i < *ptr_count_switches; i++)
    {
        printf("Switch id: %s \ ports: %d\n", ptr_switches[i]->switx_number, ptr_switches[i]->ports_quantity);
    }

    WriteXmlInstance(ptr_count_switches, ptr_switches);

    free(ptr_count_switches);
    free(ptr_switches);
}

/*
int main()
{
    //*****************+****** VARIABLES **********************

	//Variables for getting a word
	char string[150];                   //Max length of a word
	char *word = string;                 //Contains the last read word
	int  length_word = 1;                   //Length of last read word
	char *aux_word;                     //Used to copy the read words in other addresses

	switx **ptr_switches;               // Pointer to pointers of switch structure
	int switches_quantity = 0;          //Number of switches
	switx *ptr_switch;                  //Pointer to a switch
	int aux_ptr_switch;                 // Auxiliary variable to can use the function get_Array_int with ptr_switches
	port *ptr_port;                    //Pointer a port

	unsigned long aux_hypercycle;

	// Strings for comparing with read words, convert to macros??
	char label_switch[] = "switch:";
	int length_label_switch = 7;
	char label_portnumber[] = "port_number:";
	int length_label_portnumber = 12;

	int last_port;
	int aux_acll;
	int i;
	int j;
	int k;
	int block = 1;

	//Read terminal
	char str[100];
	char str1[100];
	//char *str;
	//str = malloc(sizeof(char)*100);



	//****** MYSQL ******
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *server = "localhost";
	char *user = "root";
    char *password = "12345678"; // Poner la contraseña
	char *database = "mydata";
	char *myquery;
	int list_number = 1;  //indicates which the list

	//Variables to can send queries to the database
	unsigned long  aux_period;
	int aux_gates_state;//conn = mysql_init(NULL);
	//myquery = malloc(sizeof(char)*200);
	char *aux_switx_number;
	int aux1;
	int aux_port_number;

	//Variables for reading the chosen config from the database
	switx chosen_switx;
	port chosen_port;
	int chosen_port_number;
	int chosen_switx_number;
	int chosen_list_number;
	unsigned long chosen_period;
	int chosen_gates_state;
	unsigned long chosen_hypercycle;
	char c;
	myquery = malloc(sizeof(char)*200);

	int error = 0;

	//Variables to create the XML file
	char s64[] = "64";
	char s65[] = "65";
	char s66[] = "66";
	char s67[] = "67";
	char name_file[13];


	int count_switches = 0; // indicates the quantity of switches in the reading file

  	while(1)
  	{

    	//Read word from terminal
	    scanf("%s",str); //doesn't take into account BUFFER OVERLFOW. TODO

		if((strcmp(str,"store") == 0) || (strcmp(str,"store-upload") == 0) || (strcmp(str,"upload") == 0))
	    {

			//printf("[INFO] Command: store, store-upload or upload\n");
			//Only connect to the database is we want to store lists
			if((strcmp(str,"store") == 0) || (strcmp(str,"store-upload") == 0))
			{
				conn = mysql_init(NULL);
                // Connect to database
		        if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) 
				{
		            fprintf(stderr, "%s\n", mysql_error(conn));
		            exit(1);
		        }
			}

            //************** INITIALIZATION******************
			ptr_switches = malloc(sizeof(ptr_switch)*NUMBER_OF_SWITCHS);

            //********* START READING THE FILE*********

			FILE * fpointer = fopen("mytext.txt","r");

			while(1)
			{
				getWord(fpointer, word, &length_word, 0);
				
				if(length_word == END_FILE)
				{
					break;
				}


                //******* GET A SWITCH *************
				if(compareWords(word, length_word,label_switch, length_label_switch, 0) == TRUE)
				{
					count_switches++;
					//printf("[INFO] ************************ SWITCH: %d *******************\n", switches_quantity+1 );

					getWord(fpointer, word, &length_word, 0);                                                           //Read the switch number

					ptr_switch  = malloc(sizeof(switx));                                                                // Space for the structure switx
					aux_word = copyCharArray(word,length_word);                                                         //Find an address to store the switch number
					ptr_switch->switx_number = aux_word;
					ptr_switch->length_switx_number = length_word;                                                      //When we get numbers te function returns length +1
					ptr_switch->ports_quantity = 0;                                                                     //Initialise the number of ports
					ptr_switches[switches_quantity] = ptr_switch;                                                       //Store the new switch in the array of switches
					ptr_switches[switches_quantity]->ports = malloc(sizeof(ptr_port)*NUMBER_OF_PORTS);                  //Initialise the space for the ports
					switches_quantity++;                                                                                //Update the number of switches


					//***** Database ****
					//Only use the database if it is connected
					if((strcmp(str,"store") == 0) || (strcmp(str,"store-upload") == 0))
					{
						//printf("ERROR \n");
						myquery = malloc(sizeof(char)*200);
						sprintf(myquery, "select list_number, switch_number  from info group by switch_number, list_number HAVING switch_number=%d order by list_number DESC", atoi(aux_word));
						//Query to find the last list_number
						if (mysql_query(conn, myquery))
						{
							fprintf(stderr, "%s\n", mysql_error(conn));
							exit(1);
						}
						MYSQL_RES *query_results = mysql_store_result(conn);
						if ((row = mysql_fetch_row(query_results))!=NULL)
						{
							list_number = row[0] ? atoi(row[0]) : 0;
							//printf("[DATABASE]LIST NUMBER: %d\n", list_number);
							list_number++;
						}
						else
						{
							list_number = 1;
						}

						//printf("[DATABASE] NEW LIST NUMBER: %d\n", list_number);
					}
	
                    //*****DEBUG*********
					//printf("[DEBUG] Address switch %d: %p\n",switches_quantity - 1 , ptr_switches[switches_quantity - 1]);
					//printf("[INFO] Switch number: %s \n",ptr_switches[switches_quantity - 1]->switx_number);
					block = 1;

				}


                //******* GET A PORT **********

				if(compareWords(word,length_word,label_portnumber,length_label_portnumber,0) == TRUE)
				{

					//printf("[INFO] +++++++ PORT: %d +++++++\n", ptr_switches[switches_quantity-1]->ports_quantity+1 );

					getWord(fpointer, word, &length_word, 0);                                                      //Read the port number
					ptr_port = malloc(sizeof(port));                                                               // Space for the structure port
					aux_word = copyCharArray(word, length_word);                                                   //Store the port number in  a free address
					ptr_port->port_number = atoi(aux_word);                                                       //Store the port number in the structure, we need to convert it to an integer
					ptr_port->admin_control_list_length = 0;                                                      //Initialise the length of the list
					aux_hypercycle = 0;                                                                           //Initialise the value of the hypercycle

					last_port = ptr_switches[switches_quantity-1]->ports_quantity;                                //Store the number of ports in a variable to make the code more readable
					ptr_switches[switches_quantity-1]->ports[last_port] = ptr_port;                               //Store the new switch in the array of switches
					ptr_switches[switches_quantity-1]->ports_quantity++;                                          //Update the number of ports in the switch
					ptr_switches[switches_quantity-1]->ports[last_port]->period = malloc(sizeof(unsigned long)*NUMBER_OF_LISTS);        //Allocate memory for the lists of the port
					ptr_switches[switches_quantity-1]->ports[last_port]->gates_state = malloc(sizeof(unsigned long)*NUMBER_OF_LISTS);

					block = 0;                                                                                    //If the port have been read then we can proceed  to read the configuration
					getWord(fpointer, word, &length_word, 0);                                                     //Read the period

          //printf("[INFO] Port number: %d \n",ptr_switches[switches_quantity-1]->ports[last_port]->port_number);
				}

                //******* GET PERIOD AND GATES-STATE ********
				//If it's not a port or a switch it should be the period and the gates state

			if(block == 0)
			{
			//printf("[INFO] ---- NEW LIST----\n");

				aux_word = copyCharArray(word, length_word);
				aux_acll = ptr_switches[switches_quantity-1]->ports[last_port]->admin_control_list_length;              //Store the length of the list in a variable to make the code more readable
				ptr_switches[switches_quantity-1]->ports[last_port]->period[aux_acll] = atol(aux_word);                 //Convert the period into a double and store it
				aux_hypercycle = aux_hypercycle + atol(aux_word);                                                       //Calculate the new hypercycle
				ptr_switches[switches_quantity-1]->ports[last_port]->hypercycle = (float)aux_hypercycle/1000000000;     //Convert the hypercyle to seconds and store it

				getWord(fpointer, word, &length_word, 0);                                                               //Get the gates state
				ptr_switches[switches_quantity-1]->ports[last_port]->gates_state[aux_acll] = BinCharToInt(word, length_word); //Store the gates state
				ptr_switches[switches_quantity-1]->ports[last_port]->admin_control_list_length++;                       //Update the length of the list



				//printf("[INFO] Admin control list length: %d \n",ptr_switches[switches_quantity-1]->ports[last_port]->admin_control_list_length);
				//printf("[INFO] Hypercycle: %2f \n",ptr_switches[switches_quantity-1]->ports[last_port]->hypercycle);
				//printf("[INFO] Period: %li \n",ptr_switches[switches_quantity-1]->ports[last_port]->period[aux_acll]);
				//printf("[INFO] Gates state: %d \n",ptr_switches[switches_quantity-1]->ports[last_port]->gates_state[aux_acll]);


                //******** Database ********

				if((strcmp(str,"store") == 0) || (strcmp(str,"store-upload") == 0))
				{
					aux_period = ptr_switches[switches_quantity-1]->ports[last_port]->period[aux_acll];
					aux_gates_state = ptr_switches[switches_quantity-1]->ports[last_port]->gates_state[aux_acll];
					aux_switx_number = copyCharArray(ptr_switches[switches_quantity-1]->switx_number, ptr_switches[switches_quantity-1]->length_switx_number);
					aux1 = atoi(aux_switx_number);
					aux_port_number = (int)ptr_switches[switches_quantity-1]->ports[last_port]->port_number;
					//printf("aux port number : %d\n",aux_port_number);
					//list_number = 1;

					sprintf(myquery, "INSERT INTO info(switch_number,port_number,list_number,index_,period,gates_state) VALUES(%d, %d, %d, %d, %li, %d)",aux1, aux_port_number,list_number, aux_acll, aux_period, aux_gates_state);

					//printf("[DATABASE] Query\n");
					if (mysql_query(conn, myquery)) 
					{
	                	fprintf(stderr, "%s\n", mysql_error(conn));
						exit(1);
					}
					//printf("[DATABASE] Configuration added in database\n");
				}

			}

		}

		//We can't disconnect from the database if we are not connected
		if((strcmp(str,"store") == 0) || (strcmp(str,"store-upload") == 0))
		{
        	mysql_close(conn);
		}
		fclose(fpointer);

		switches_quantity = 0;
		block = 1;
	}
    else
	{
		error++;
	}

    if((strcmp(str,"store-upload") == 0) || (strcmp(str,"upload") == 0) || (strcmp(str,"upload-config") == 0))
    {
        //********** WRITE XML FILE **************
		//printf("[INFO] Writing XML file\n");

		if((strcmp(str,"store-upload") == 0) || (strcmp(str,"upload") == 0))
		{	
			for (k = 0; k < count_switches; k++)
			{
				if(strcmp(ptr_switches[k]->switx_number,s64) == 0 || strcmp(ptr_switches[k]->switx_number,s65) == 0 )
				{
					sprintf(name_file,"XMLdata%s.xml",ptr_switches[k]->switx_number);
					FILE * fpointer1 = fopen(name_file,"w");
					fprintf(fpointer1,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>>\n");
					fprintf(fpointer1,"<if:interfaces xmlns:if=\"urn:ietf:params:xml:ns:yang:ietf-interfaces\">\n");

					//printf("[INFO] Command: store-upload or upload\n");
					//printf("%s\n",name_file);
					for(i = 0; i < ptr_switches[k]->ports_quantity; i++)
					{
						writePort(fpointer1, ptr_switches[k]->ports[i]->port_number, ptr_switches[k]->ports[i]->admin_control_list_length);
						writeList(fpointer1, ptr_switches[k]->ports[i]->period, ptr_switches[k]->ports[i]->gates_state, ptr_switches[k]->ports[i]->hypercycle, ptr_switches[k]->ports[i]->admin_control_list_length);
        	    	}
					fprintf(fpointer1,"</if:interfaces>\n");
					fclose(fpointer1);
				}
			}

		}
    	else
		{
			sprintf(name_file,"XMLdata%i.xml",chosen_switx_number);
        	FILE * fpointer1 = fopen(name_file,"w");
        	fprintf(fpointer1,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>>\n");
        	fprintf(fpointer1,"<if:interfaces xmlns:if=\"urn:ietf:params:xml:ns:yang:ietf-interfaces\">\n");
        	//printf("[INFO] Command: upload-config\n");
        	//printf("%s\n",name_file);
        	for(i = 0; i < chosen_switx.ports_quantity; i++)
        	{
				writePort(fpointer1, chosen_switx.ports[i]->port_number, chosen_switx.ports[i]->admin_control_list_length);
				writeList(fpointer1, chosen_switx.ports[i]->period, chosen_switx.ports[i]->gates_state, chosen_switx.ports[i]->hypercycle, chosen_switx.ports[i]->admin_control_list_length);
        	}
        	fprintf(fpointer1,"</if:interfaces>\n");
        	fclose(fpointer1);
		}

		break;
	}
	else
	{
		error++;
	}

    // choose-config --switch 11111 --list 2
	if(strcmp(str,"choose-config") == 0)
	{
		//printf("[INFO] Command: choose-config\n");
		conn = mysql_init(NULL);
		myquery = malloc(sizeof(char)*200);
        //* Connect to database
		if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) 
		{
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}


		scanf("%s",str);
		if(strcmp(str,"--switch") == 0)           //Check syntax
		{
			scanf("%s",str);                        //get the switch number
			chosen_switx_number = atoi(str);
			scanf("%s",str);
			if(strcmp(str,"--list") == 0)           //Check syntax
			{
				scanf("%s",str);                      //get the list number
				chosen_list_number = atoi(str);
				//printf("[DATABASE]  Query\n");
				//printf("[DATABASE]  select port_number,period, gates_state from info where switch_number=%d and list_number=%d\n", chosen_switx_number, chosen_list_number);
				sprintf(myquery, "select port_number,period, gates_state from info where switch_number=%d and list_number=%d order by port_number, index_ asc", chosen_switx_number, chosen_list_number);
				//printf("EEROROROROR\n");
				if (mysql_query(conn, myquery)) 
				{
					fprintf(stderr, "%s\n", mysql_error(conn));
					exit(1);
				}

				//printf("[DATABASE]  Query done\n");

				MYSQL_RES *query_chosen = mysql_store_result(conn);
				i = 0;
				j = 0;
				row = mysql_fetch_row(query_chosen);
				chosen_switx.ports = malloc(sizeof(ptr_port)*NUMBER_OF_PORTS);                     //Allocate memory for the ports
				chosen_switx.ports[i] = malloc(sizeof(port));                                      //Allocate memory for the first port
				chosen_switx.ports[i]->period = malloc(sizeof(unsigned long)*NUMBER_OF_LISTS);    //Allocate memory for the periods
				chosen_switx.ports[i]->gates_state = malloc(sizeof(int)*NUMBER_OF_LISTS);        //Allocate memory for the gates state
				chosen_port_number = row[0] ? atoi(row[0]) : 0;
				chosen_switx.ports[i]->port_number = chosen_port_number;                         //Get the first port
				chosen_period = row[1] ? atol(row[1]) : 0;                                       //first gates state and period
				chosen_switx.ports[i]->period[j] = chosen_period;
				chosen_gates_state = row[2] ? atoi(row[2]) : 0;
				chosen_switx.ports[i]->gates_state[j] = chosen_gates_state;

				chosen_switx.ports_quantity = 1;
				chosen_switx.ports[i]->admin_control_list_length = 1;
				chosen_hypercycle = chosen_period;
	
				//printf("Port number: %d, Period: %li, Gates state: %d\n",chosen_switx.ports[i]->port_number, chosen_switx.ports[i]->period[j], chosen_switx.ports[i]->gates_state[j] );

				//printf("Index: %d Hypercyle(calculating): %li\n",chosen_switx.ports[i]->admin_control_list_length-1,chosen_hypercycle );
				j++;


				while ((row = mysql_fetch_row(query_chosen))!=NULL)
				{
					chosen_port_number = row[0] ? atoi(row[0]) : 0;
					if(chosen_port_number != chosen_switx.ports[i]->port_number)             //Chek if it is a new port
					{

						chosen_switx.ports[i]->hypercycle = (float)chosen_hypercycle/1000000000;    //calculate the hypercyle of the last read port

						// printf("[DATABASE] Read port: %d\n",chosen_switx.ports[i]->port_number);
						//printf("Index: %d Hypercyle(final): %2f\n",chosen_switx.ports[i]->admin_control_list_length-1,chosen_switx.ports[i]->hypercycle );
						//printf("Admin control list length: %d\n",chosen_switx.ports[i]->admin_control_list_length);

						chosen_switx.ports_quantity++;
						i++;
						j = 0;
						chosen_switx.ports[i] = malloc(sizeof(port));                                      //Allocate memory for the port
						chosen_switx.ports[i]->port_number = chosen_port_number;                           //Store port number
						chosen_switx.ports[i]->period = malloc(sizeof(unsigned long)*NUMBER_OF_LISTS);     //Allocate memory for the periods
						chosen_switx.ports[i]->gates_state = malloc(sizeof(int)*NUMBER_OF_LISTS);          //Allocate memory for the gates state

						chosen_switx.ports[i]->admin_control_list_length = 0;
						chosen_hypercycle = 0;

					}
					chosen_period = row[1] ? atol(row[1]) : 0;
					chosen_switx.ports[i]->period[j] = chosen_period;
					chosen_gates_state = row[2] ? atoi(row[2]) : 0;
					chosen_switx.ports[i]->gates_state[j] = chosen_gates_state;

					chosen_hypercycle = chosen_hypercycle + chosen_period;
					chosen_switx.ports[i]->admin_control_list_length++;

					//printf("Port number: %d, Period: %li, Gates state: %d\n",chosen_switx.ports[i]->port_number, chosen_switx.ports[i]->period[j], chosen_switx.ports[i]->gates_state[j] );
					//printf("Index: %d Hypercyle(calculating): %li\n",chosen_switx.ports[i]->admin_control_list_length-1,chosen_hypercycle );
					j++;

				}
				chosen_switx.ports[i]->hypercycle = (float)chosen_hypercycle/1000000000;    //calculate the hypercyle of the last read port
				//printf("Index: %d Hypercyle(final): %2f\n",chosen_switx.ports[i]->admin_control_list_length-1,chosen_switx.ports[i]->hypercycle );
				//printf("Admin control list length: %d\n",chosen_switx.ports[i]->admin_control_list_length);
			}

        }
		mysql_close(conn);
	}
    else
	{
		error++;
	}

    // select
    // select --switch [switch number] --list [list number]
    if(strcmp(str,"select") == 0)
	{
		//printf("[INFO] Command: select\n");

		conn = mysql_init(NULL);
		myquery = malloc(sizeof(char)*200);
        // Connect to database
		if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) 
		{
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}

		c = fgetc(stdin);

		if(c == ' ')
		{
			//printf("Show --switch --list\n");
			scanf("%s",str);    //Read --switch
			scanf("%s",str);    //Read switch number
			scanf("%s",str1);    //Read --list
			scanf("%s",str1);    //Read list number

			sprintf(myquery,"SELECT  port_number, index_, period, gates_state from info where switch_number = %s and list_number = %s order by port_number, index_ asc", str, str1);
			if (mysql_query(conn, myquery)) 
			{
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
			}

			res = mysql_use_result(conn);

            // output table name
			printf("Port number | Index |    Period    | Gates state \n");
			printf("------------|-------|--------------|-------------\n");
			while ((row = mysql_fetch_row(res)) != NULL)
			printf("    %s         %s        %s           %s \n", row[0],row[1],row[2], row[3]);

		}
		//Show number of lists for each switch
		else if(c == '\n')
		{
			myquery = "select distinct list_number, switch_number from info order by switch_number";

			if (mysql_query(conn, myquery)) 
			{
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
			}

			res = mysql_use_result(conn);

            // output table name
			printf("   List ID    | Switch number\n");
			printf("--------------|-------------\n");
			while ((row = mysql_fetch_row(res)) != NULL)
			printf("       %s           %s\n", row[0],row[1]);

		}

	mysql_close(conn);

    }
    else
	{
		error++;
	}

    //delete
    //delete --switch
    //delete --switch --list
    if(strcmp(str,"delete") == 0)
	{

		//printf("[INFO] Command: delete\n");
		conn = mysql_init(NULL);
		myquery = malloc(sizeof(char)*200);
		
        // Connect to database
		if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) 
		{
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}

		c = fgetc(stdin);
		if(c == ' ')
		{
			scanf("%s",str);    //Read --switch
			scanf("%s",str);    //Read switch number
			c = fgetc(stdin);


			if (c == ' ')
	        {
				scanf("%s",str1);    //Read --list
				scanf("%s",str1);    //Read list number
				sprintf(myquery,"DELETE FROM info where switch_number = %s and list_number = %s",str, str1);
			
				if (mysql_query(conn, myquery)) 
				{
					fprintf(stderr, "%s\n", mysql_error(conn));
					exit(1);
				}
			}
			else
			{
				sprintf(myquery,"DELETE FROM info where switch_number = %s",str);
				if (mysql_query(conn, myquery)) 
				{
					fprintf(stderr, "%s\n", mysql_error(conn));
					exit(1);
				}

			}

		}
		//Delete all the lists
		else if(c == '\n')
		{
			printf("Delete\n");
			myquery = "DELETE FROM info";
			if (mysql_query(conn, myquery))
			{
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
			}

		}

		mysql_close(conn);

    }
    else
	{	
		error++;
	}

		if(strcmp(str,"help") == 0)
		{
		  printf("store                       ->store the new lists in the database\n");
		  printf("store-upload                ->store the new lists and upload the configuration\n");
		  printf("upload                      ->upload the configuration from the file\n");
		  printf("upload-config               ->upload the chosen configuration from the database\n");
		  printf("choose-config --switch [switch ID] --list [list ID]   ->choose the configuration to be uploaded\n");
		  printf("select                      ->Show the lists of each switch\n");
		  printf("select --switch [switch ID] --list [list ID]  ->show the chosen list of a switch \n");
		  printf("delete                      ->remove all the infomation in the database\n");
		  printf("delete --switch [switch ID] ->remove the all the lists from the switch\n");
		  printf("delete --switch [switch ID] --list [list ID] ->remove the desired list from the database\n");

		}
		else
		{
			error++;
		}

		if(error == 6)
		{
			printf("[ERROR] Unknown command\n");
		}
		error = 0;
	}

	return 0;
}
*/
