// Dennies Anwar
// Implementation of DFS, BFS, DEF, and a custom algorithm to find path to solve maze.
#include "Maze.cpp"
#include <stack>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace std;

namespace solutions
{

	int getNumberOfWalls(MazeNode* a_node)
	{
		int walls = 0;
		for (directions::nesw dir = directions::NORTH; dir < directions::EAST; dir = directions::nesw(dir + 1))
		{
			if (a_node->getDirectionNode(dir) == nullptr || a_node->getDirectionNode(dir)->isWall() || a_node->getDirectionNode(dir)->isVisited())
			{
				walls++;
			}
		}
		return walls;
	}

	bool canTravel(MazeNode* a_node)
	{
		if (a_node->isVisited() || a_node->isWall())
		{
			return false;
		}
		return true;
	}

	std::vector<MazeNode> solveDFS(Maze& a_maze)
	{
		stack <MazeNode*> s;
		unordered_map<MazeNode*, MazeNode*> parentMap; // Map to backtrack for path
		vector<MazeNode> path;
		s.push(a_maze.getFirstNode());

		while (!s.empty()) {
			MazeNode* curr = s.top();
			s.pop();
			if (curr->isWall() || curr->isVisited()) {
				continue;
			}
			if (curr->isVisited() == false) {
				curr->setVisited();
			}

			// Goes through each direction and makes sure if we can travel to it.
			// If so we add it to the stack and keep track of its parent in the parentMap.
			for (int i = 0; i < 4; i++) {
				directions::nesw dir = static_cast<directions::nesw>(i);
				if (curr->getDirectionNode(dir) != nullptr && canTravel(curr->getDirectionNode(dir))) {
					s.push(curr->getDirectionNode(dir));
					parentMap[curr->getDirectionNode(dir)] = curr;
				}
			}
		}

		// Creates path by going back from last node parent by parent to start.
		MazeNode* curr = a_maze.getLastNode();
		while (parentMap.find(curr) != parentMap.end()) {
			path.insert(path.begin(), *curr);
			curr = parentMap[curr];
		}
		path.insert(path.begin(), *a_maze.getFirstNode()); // Since first node is not in map we have to manually insert it here

		return path;
	}

	std::vector<MazeNode> solveBFS(Maze& a_maze)
	{
		queue <MazeNode*> q;
		unordered_map<MazeNode*, MazeNode*> parentMap; // Map to backtrack for path
		vector<MazeNode> path;
		q.push(a_maze.getFirstNode());

		while (!q.empty()) {
			MazeNode* curr = q.front();
			q.pop();
			if (curr->isWall() || curr->isVisited()) {
				continue;
			}
			if (curr->isVisited() == false) {
				curr->setVisited();
			}

			// Goes through each direction and makes sure if we can travel to it.
			// If so we add it to the stack and keep track of its parent in the parentMap.
			for (int i = 0; i < 4; i++) {
				directions::nesw dir = static_cast<directions::nesw>(i);
				if (curr->getDirectionNode(dir) != nullptr && canTravel(curr->getDirectionNode(dir))) {
					q.push(curr->getDirectionNode(dir));
					parentMap[curr->getDirectionNode(dir)] = curr;
				}
			}

		}

		// Creates path by going back from last node parent by parent to start.
		MazeNode* curr = a_maze.getLastNode();
		while (parentMap.find(curr) != parentMap.end()) {
			path.insert(path.begin(), *curr);
			curr = parentMap[curr];
		}
		path.insert(path.begin(), *a_maze.getFirstNode()); // Since first node is not in map we have to manually insert it here

		return path;
	}

	int numberOfWalls(MazeNode* node) {
		int count = 0;
		for (int i = 0; i < 4; i++) {
			directions::nesw dir = static_cast<directions::nesw>(i);
			MazeNode* temp = node->getDirectionNode(dir);
			if (temp == nullptr || !canTravel(temp)) {
				count++;
			}
		}
		return count;
	}

	std::vector<MazeNode> solveDEF(Maze& a_maze)
	{
		vector<MazeNode> path;
		stack<MazeNode*> dfsS;

		// Used set only when searching for deadends. Did not want to intefere isVisited which is used later.
		unordered_set<MazeNode*> visitedNodes;
		stack<MazeNode*> deadEnds;

		dfsS.push(a_maze.getFirstNode());

		// DFS to find all deadends. Looping through getNodes() gave problems.
		while (!dfsS.empty()) {
			MazeNode* curr = dfsS.top();
			dfsS.pop();
			if (curr->isWall() || visitedNodes.find(curr) != visitedNodes.end()) {
				continue;
			}

			// Makes sure dead end that its not first or last node.
			if (numberOfWalls(curr) == 3 && curr != a_maze.getFirstNode() && curr != a_maze.getLastNode()) {
				deadEnds.push(curr);
			}

			for (int i = 0; i < 4; i++) {
				directions::nesw dir = static_cast<directions::nesw>(i);
				if (curr->getDirectionNode(dir) != nullptr && canTravel(curr->getDirectionNode(dir))) {
					dfsS.push(curr->getDirectionNode(dir));
				}
			}
			visitedNodes.insert(curr);
		}


		// Goes through each deadend marks each node visited until it gets to a junction.
		while (!deadEnds.empty()) {
			MazeNode* dEnd = deadEnds.top();
			deadEnds.pop();

			// 3 possible directions means a junction
			while (numberOfWalls(dEnd) > 2) {
				dEnd->setVisited();
				if (dEnd->getDirectionNode(directions::NORTH) != nullptr && canTravel(dEnd->getDirectionNode(directions::NORTH))) {
					dEnd = dEnd->getDirectionNode(directions::NORTH);
				}
				else if (dEnd->getDirectionNode(directions::SOUTH) != nullptr && canTravel(dEnd->getDirectionNode(directions::SOUTH))) {
					dEnd = dEnd->getDirectionNode(directions::SOUTH);
				}
				else if (dEnd->getDirectionNode(directions::EAST) != nullptr && canTravel(dEnd->getDirectionNode(directions::EAST))) {
					dEnd = dEnd->getDirectionNode(directions::EAST);
				}
				else if (dEnd->getDirectionNode(directions::WEST) != nullptr && canTravel(dEnd->getDirectionNode(directions::WEST))) {
					dEnd = dEnd->getDirectionNode(directions::WEST);
				}
				else {
					break;
				}
			}
		}

		// Goes through unvisted nodes from first node to last adding them path vector.
		MazeNode* curr = a_maze.getFirstNode();
		while (curr != a_maze.getLastNode()) {
			path.insert(path.end(), *curr);
			curr->setVisited();
			for (int i = 0; i < 4; i++) {
				directions::nesw dir = static_cast<directions::nesw>(i);
				MazeNode* temp = curr->getDirectionNode(dir);
				if (temp != nullptr && canTravel(temp)) {
					curr = temp;
					break;
				}
			}
		}
		path.insert(path.end(), *a_maze.getLastNode()); // Make sure to insert last node

		return path;
	}

	bool customHelper(Maze& a_maze, MazeNode* node, vector<MazeNode>& path) {
		if (node == nullptr || !canTravel(node)) {
			return false;
		}
		node->setVisited();
		if (node == a_maze.getLastNode()) {
			path.insert(path.begin(), *node);
			return true;
		}
		if (customHelper(a_maze, node->getDirectionNode(directions::NORTH), path)) {
			path.insert(path.begin(), *node);
			return true;
		}
		if (customHelper(a_maze, node->getDirectionNode(directions::EAST), path)) {
			path.insert(path.begin(), *node);
			return true;
		}
		if (customHelper(a_maze, node->getDirectionNode(directions::SOUTH), path)) {
			path.insert(path.begin(), *node);
			return true;
		}
		if (customHelper(a_maze, node->getDirectionNode(directions::WEST), path)) {
			path.insert(path.begin(), *node);
			return true;
		}
		return false;
	}

	std::vector<MazeNode> solveCustom(Maze& a_maze)
	{
		vector<MazeNode> path;
		bool result = customHelper(a_maze, a_maze.getFirstNode(), path);
		return path;
	}

	void printPath(vector<MazeNode> path) {
		for (int i = 0; i < path.size(); i++) {
			cout << path.at(i).getStrPos() << "->";
		}
		cout << endl;
	}

	// Tests outputs of each algorithm with the three mazes.
/*	void testing() {
		cout << "Maze 1" << endl;
		Maze maze1_DFS = Maze("maze_1.csv");
		Maze maze1_BFS = Maze("maze_1.csv");
		Maze maze1_DEF = Maze("maze_1.csv");
		Maze maze1_CUS = Maze("maze_1.csv");
		cout << "DFS: ";
		printPath(solveDFS(maze1_DFS));
		cout << "BFS: ";
		printPath(solveDFS(maze1_BFS));
		cout << "DEF: ";
		printPath(solveDFS(maze1_DEF));
		cout << "CUS: ";
		printPath(solveDFS(maze1_CUS));


		cout << "\nMaze 2" << endl;
		Maze maze2_DFS = Maze("maze_2.csv");
		Maze maze2_BFS = Maze("maze_2.csv");
		Maze maze2_DEF = Maze("maze_2.csv");
		Maze maze2_CUS = Maze("maze_2.csv");
		cout << "DFS: ";
		printPath(solveDFS(maze2_DFS));
		cout << "BFS: ";
		printPath(solveDFS(maze2_BFS));
		cout << "DEF: ";
		printPath(solveDFS(maze2_DEF));
		cout << "CUS: ";
		printPath(solveDFS(maze2_CUS));


		cout << "\nMaze 3" << endl;
		Maze maze3_DFS = Maze("maze_3.csv");
		Maze maze3_BFS = Maze("maze_3.csv");
		Maze maze3_DEF = Maze("maze_3.csv");
		Maze maze3_CUS = Maze("maze_3.csv");
		cout << "DFS: ";
		printPath(solveDFS(maze3_DFS));
		cout << "BFS: ";
		printPath(solveDFS(maze3_BFS));
		cout << "DEF: ";
		printPath(solveDFS(maze3_DEF));
		cout << "CUS: ";
		printPath(solveDFS(maze3_CUS));
	}*/

}// namespace solutions
