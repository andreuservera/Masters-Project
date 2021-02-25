void main()
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
            }

        }

        fclose(fpointer);

        switches_quantity = 0;
        block = 1;


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
}
