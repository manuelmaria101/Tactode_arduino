#define TIMEOUT 5000

typedef struct CmdMove {
  int t;
  int vel;
} cmdMove;
typedef struct CmdWait {
  int t;
  char wait;
} cmdWait;

typedef struct Tile {
  char type;
  union Data {
    cmdMove Move;
    cmdWait Wait;
  } data;
} tile;

/*-----------------------------------------------------DECLARE VARIABLES---------------------------------------------*/

tile my_tile;
char program[] = "M 1000 50\nR 500 40\nM 500 -50\nS";
int tis = 0, currTime = 0, prevTime = 0, i = 0, tile_time = TIMEOUT;
byte oldState = 0, auxState = 0, next = 0;


/*-----------------------------------------------------END OF VARIABLE DECLARATION---------------------------------------------*/


void setup()
{
  Serial.begin(115200);
}

void loop() {

  if (next == 0)
  {
    //Serial.println("HERE");
    readNewTile();
    /*if (my_tile.type == 'F')
    {
      moveRobot(my_tile.data.Move.vel, 0);
    }
    else if (my_tile.type == 'S')
    {
      moveRobot(0, 0);
    }
    else if ( my_tile.type == 'W')
    {
      moveRobot(0, my_tile.data.Move.vel);
    }*/
    next = 1;
  }
  else if ((tis >= TIMEOUT) || (tis >= tile_time))
  {
    next = 0;
  }




  /*------------------DEBUG----------------*/

  Serial.print(F("OldState: "));
  Serial.print(oldState);

  Serial.print(F("        tis: "));
  Serial.print(tis);

  Serial.print(F("        TYPE: "));
  Serial.print(my_tile.type);

  Serial.print(F("        t: "));
  Serial.print(my_tile.data.Move.t);

  Serial.print(F("        vel: "));
  Serial.print(my_tile.data.Move.vel);
  Serial.println();

  delay(500);
  refreshTimer();

}

/*-----------------------------------------------------FUNCTIONS---------------------------------------------*/

void readNewTile(void)
{
  char trash;
  char line[strlen(program)];
  int j;
  j = 0;
  auxState = oldState + 1;
  setState(auxState);
  for ( ; program[i] != '\0'; i++)
  {
    if (program[i] == '\n')
    {
      i++;
      break;
    }
    else
    {
      line[j] = program[i];
      j++;
    }
  }

  sscanf(line, "%c", &my_tile.type);

  if (my_tile.type == 'M')
  {
    sscanf(line, "%c %d %d", &trash, &my_tile.data.Move.t, &my_tile.data.Move.vel);
    tile_time = my_tile.data.Move.t;
  }
  else if (my_tile.type == 'R')
  {
    sscanf(line, "%c %d %d", &trash, &my_tile.data.Move.t, &my_tile.data.Move.vel);
    tile_time = my_tile.data.Move.t;
  }
  else if (my_tile.type == 'S')
  {
    my_tile.data.Move.t = 0;
    my_tile.data.Move.vel = 0;
  }

}


void refreshTimer(void)
{
  currTime = millis();
  tis = tis + (currTime - prevTime);
  prevTime = currTime;
}

/*void moveRobot(float Vnom, float Wnom)
{
  robot.v = Vnom;
  robot.w = Wnom;
}*/


void setState(byte new_state)
{
  tis = 0;
  oldState = new_state;
}
