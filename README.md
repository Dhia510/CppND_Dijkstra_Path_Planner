# CppND-CppND_Dijkstra_Path_Planner
Starter code for the path planner using dijkstra algorithm
The project is designed to find the shortest path between desks in a company provided the company map represented as a weighted graph
where nodes are desks and the corridors are edges.
## Create Desks 
For this part I used a set of QLineEdit to callect the desk name, a label to indicate to user the purpose of this field and a QPushButton that will create a new desk with that name once clicked,
In order to track the created desk I added a QTextBrowser to display created desks 
The user can type a desk name in the white field then clic on the push button to create a desk
![image](https://github.com/user-attachments/assets/3bbc831c-30e5-4a63-be76-a074c32044ab)
![image](https://github.com/user-attachments/assets/0c471132-ce7b-473e-910d-03cf8e21cea0)

## Create corridors
For this part I used the same set of widgets as for the create desk feature,
The user can type a desk’s name in the first field then the second desk’s name, afterward he specifys the distance between them in the last field and clic on the push button to create the corridor
A message is displayed to indicate the corridor creation
![image](https://github.com/user-attachments/assets/77ce672c-bed2-481a-ab31-82610bffea6e)
![image](https://github.com/user-attachments/assets/eb7fa1dc-4d46-4624-9ea5-6953739c7e6e)

## Select start desk and end desk and start the path finding process Corridors
For this part I used the same set of widgets as for the the other features.
The user can type a start desk name in the first field next to « From » and in the second field next to « to » he insert the target desk and clics on find path to start searching
The shortest path will be displayed in the QTextBrowser in addition to the distance if it is found
![image](https://github.com/user-attachments/assets/93845cfc-4ba6-4b05-bd40-f5a133c96f86)





