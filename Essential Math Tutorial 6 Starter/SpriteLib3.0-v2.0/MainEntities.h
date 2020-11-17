#ifndef __MAINENTITIES_H__
#define __MAINENTITIES_H__

class MainEntities
{
public:
	//Getters
	static int MainCamera();
	static int MainPlayer();
	static int MainBall();
	static int MainTriangle();
	static int MainSquare();
	static int MainPentagon();
	static int MainHexagon();
	static int MainHeptagon();
	static int MainOctagon();
	//Setters
	static void MainCamera(int main);
	static void MainPlayer(int main);
	static void MainBall(int main);
	static void MainTriangle(int main);
	static void MainSquare(int main);
	static void MainPentagon(int main);
	static void MainHexagon(int main);
	static void MainHeptagon(int main);
	static void MainOctagon(int main);

private:
	static int m_mainCamera;
	static int m_mainPlayer;
	static int m_mainBall;
	static int m_mainTriangle;
	static int m_mainSquare;
	static int m_mainPentagon;
	static int m_mainHexagon;
	static int m_mainHeptagon;
	static int m_mainOctagon;
};

#endif // !__MAINENTITIES_H__
