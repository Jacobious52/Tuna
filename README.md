# Tuna
Visual Testing and  Debugging

Tool for programming testing and debugging purposes.

Have a visual web interface that updates tests, variables and data structures in real time.

Graph variable updates over time. Display trees and graphs. View logging infomation.

Works via three components.

1. The API. The api is planned for C++, Go, Java and Python (In that order). It will provide a simple seemless interface between writing debug code and pushing to the server

2. The Server. Writen in Go, it hosts and talks to the webpage and also listens for updates from the API. Simple just run it to start

3. The webpage. Written in Html/css/javascript, it uses bootstrap for themeing and shows the visual elements of debugging.

