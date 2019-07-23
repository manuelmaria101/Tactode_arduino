#define TIMEOUT 5000

typedef struct CmdMove {
  int t;
  int vel;
} cmdMove;
typedef struct CmdRotate {
  int angle;
  int vel;
} cmdRotate;

typedef struct Tile {
  char type;
  union Data {
    cmdMove Move;
    cmdWait Wait;
  } data;
} tile;

/*-----------------------------------------------------DECLARE VARIABLES---------------------------------------------*/

tile my_tile;
char program[] = "M 1000 50\nW 500 40\nM 500 -50\nS";
int  i = 0, angle = 0;
byte next = 0;


/*-----------------------------------------------------END OF VARIABLE DECLARATION---------------------------------------------*/


void setup()
{
  Serial.begin(115200);
}


void loop() {


  if (next == 0)
  {
    readNewTile();
    chooseState();
    next = 1;
  }
  else if (next != 0 && robot.state == 1 && tis >= my_tile.data.Move.t)
  {
    next = 0;
  }
  else if (next != 0 && robot.state == 2  && angle > my_tile.data.Rotate.angle)
  {
    next = 0;
  }



  if (robot.state == 1) //MOVE TILE
  {
    moveRobot(my_tile.data.Move.vel, 0);
  }
  else if (robot.state == 2) //ROTATE TILE
  {
    moveRobot(0, my_tile.data.Rotate.vel);
  }
  else if (robot.state == 0) //STOP TILE
  {
    moveRobot(0, 0);
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
    sscanf(line, "%c %d %ld", &trash, &my_tile.data.Move.t, &my_tile.data.Move.vel);
    tile_time = my_tile.data.Move.t;
  }
  else if (my_tile.type == 'W')
  {
    sscanf(line, "%c %d %ld", &trash, &my_tile.data.Rotate.angle, &my_tile.data.Rotate.vel);
  }
  else if (my_tile.type == 'S')
  {
    my_tile.data.Move.t = 0;
    my_tile.data.Move.vel = 0;
  }
}

void chooseState(void)
{
  if (my_tile.type == 'M')
  {
    setState(1);
  }
  else if (my_tile.type == 'W')
  {
    setState(2);
  }
  else if (my_tile.type == 'S')
  {
    setState(0);
  }
}
