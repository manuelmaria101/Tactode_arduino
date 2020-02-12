#define TIMEOUT 2000

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
    cmdRotate Rotate;
  } data;
} tile;

/*-----------------------------------------------------DECLARE VARIABLES---------------------------------------------*/

tile my_tile;
char program[] = "M 2000 50\nW 90 50\nM 2000 50\nW 90 50\nM 2000 50\nW 90 50\nM 2000 50\nW 90 50\nS";
int  i = 0;
float my_angle = 0;
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
    next = 1;
  }
  else if (next != 0 && robot.state == 1 && tis >= my_tile.data.Move.t)
  {
    next = 0;
  }
  else if (next != 0 && robot.state == 2  && (robot.dtheta > my_angle))
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

  Serial.print(F("Robot State: "));
  Serial.print(robot.state);

  Serial.print(F("        tis: "));
  Serial.print(tis);

  Serial.print(F("        theta: "));
  Serial.print(robot.dtheta);

  Serial.print(F("        TYPE: "));
  Serial.print(my_tile.type);

  Serial.print(F("        t: "));
  Serial.print(my_tile.data.Move.t);
  Serial.print(F("        vel: "));
  Serial.print(my_tile.data.Move.vel);

  Serial.print(F("        angle: "));
  Serial.print(my_tile.data.Rotate.angle);
  Serial.print(F("        Rotate vel: "));
  Serial.print(my_tile.data.Rotate.vel);

  Serial.println();

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
  Serial.println(line);
  if (my_tile.type == 'M')
  {
    sscanf(line, "%c %d %d", &trash, &my_tile.data.Move.t, &my_tile.data.Move.vel);
  }
  else if (my_tile.type == 'W')
  {
    sscanf(line, "%c %d %d", &trash, &my_tile.data.Rotate.angle, &my_tile.data.Rotate.vel);
    my_angle = refresh_angle(my_tile.data.Rotate.angle);
  }
  else if (my_tile.type == 'S')
  {
    my_tile.data.Move.t = 0;
    my_tile.data.Move.vel = 0;
    my_tile.data.Rotate.angle = 0;
    my_tile.data.Rotate.vel = 0;
  }
  chooseState();
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

float refresh_angle(int my_angle)
{
  float new_angle = my_angle * PI / 180;
  return new_angle;
}
