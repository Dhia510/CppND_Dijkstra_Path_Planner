#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QString>
#include <QTextBrowser>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <QDebug>
#include <queue>  // Add this line to include the priority_queue

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

std::map<std::string, std::vector<std::pair<std::string, int>>> graph;

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonCreateDesk_clicked()
{
    QString deskName = ui->lineEditDeskName->text();
    if (deskName.isEmpty()) {
        ui->textBrowserDisplay->append("Error: Desk name cannot be empty.");
        return;
    }
    std::string name = deskName.toStdString();
    if (graph.find(name) == graph.end()) {
        graph[name] = {};  // Initialize an empty adjacency list
        ui->textBrowserDisplay->append("Desk created: " + deskName);
    } else {
        ui->textBrowserDisplay->append("Error: Desk already exists.");
    }
    ui->lineEditDeskName->clear();
}


void MainWindow::on_buttonCreateLink_clicked() {
    QString desk1 = ui->lineEditDesk1->text();
    QString desk2 = ui->lineEditDesk2->text();
    QString distanceStr = ui->lineEditDistance->text();

    if (desk1.isEmpty() || desk2.isEmpty() || distanceStr.isEmpty()) {
        ui->textBrowserDisplay->append("Error: All fields must be filled.");
        return;
    }

    int distance = distanceStr.toInt();
    std::string d1 = desk1.toStdString();
    std::string d2 = desk2.toStdString();

    if (graph.find(d1) == graph.end() || graph.find(d2) == graph.end()) {
        ui->textBrowserDisplay->append("Error: One or both desks do not exist.");
        return;
    }

    // Check if the link already exists
    bool linkExists = false;
    for (const auto& edge : graph[d1]) {
        if (edge.first == d2) {
            linkExists = true;
            break;
        }
    }

    if (linkExists) {
        ui->textBrowserDisplay->append("Error: Link between " + desk1 + " and " + desk2 + " already exists.");
    } else {
        // Add the link in both directions
        graph[d1].push_back({d2, distance});
        graph[d2].push_back({d1, distance});  // Add the reverse link
        ui->textBrowserDisplay->append("Link created: " + desk1 + " - " + desk2 + " (" + distanceStr + ")");
    }

    ui->lineEditDesk1->clear();
    ui->lineEditDesk2->clear();
    ui->lineEditDistance->clear();
}

int MainWindow::dijkstra(const std::string& start, const std::string& end, std::vector<std::string>& path) {
    // Initialize distances and predecessors
    std::map<std::string, int> distances;
    std::map<std::string, std::string> predecessors;
    for (const auto& desk : graph) {
        distances[desk.first] = INT_MAX;  // Set all distances to infinity
        predecessors[desk.first] = "";    // No predecessor initially
    }
    distances[start] = 0;

    // Priority queue (min-heap) to store desks with their distances
    auto compare = [&distances](const std::string& a, const std::string& b) {
        return distances[a] > distances[b];
    };
    std::priority_queue<std::string, std::vector<std::string>, decltype(compare)> pq(compare);
    pq.push(start);

    while (!pq.empty()) {
        std::string current = pq.top();
        pq.pop();

        // Stop if we reached the target desk
        if (current == end) {
            break;
        }

        // Explore neighbors
        for (const auto& neighbor : graph[current]) {
            std::string neighborDesk = neighbor.first;
            int weight = neighbor.second;
            int newDist = distances[current] + weight;

            if (newDist < distances[neighborDesk]) {
                distances[neighborDesk] = newDist;
                predecessors[neighborDesk] = current;
                pq.push(neighborDesk);
            }
        }
    }

    // Reconstruct the shortest path
    std::string current = end;
    while (current != "") {
        path.push_back(current);
        current = predecessors[current];
    }
    std::reverse(path.begin(), path.end());

    return distances[end];
}


void MainWindow::on_buttonFindPath_clicked() {
    QString sourceDesk = ui->lineEditSourceDesk->text();
    QString targetDesk = ui->lineEditTargetDesk->text();

    if (sourceDesk.isEmpty() || targetDesk.isEmpty()) {
        ui->textBrowserDisplay->append("Error: Source and target desks must be specified.");
        return;
    }

    std::string source = sourceDesk.toStdString();
    std::string target = targetDesk.toStdString();

    // Check if desks exist
    if (graph.find(source) == graph.end() || graph.find(target) == graph.end()) {
        ui->textBrowserDisplay->append("Error: One or both desks do not exist.");
        return;
    }

    // Run Dijkstra's algorithm
    std::vector<std::string> path;
    int distance = dijkstra(source, target, path);

    // Display the results
    if (distance == INT_MAX) {
        ui->textBrowserDisplay->append("No path found between " + sourceDesk + " and " + targetDesk);
    } else {
        QString pathStr = "Shortest path: ";
        for (const auto& desk : path) {
            pathStr += QString::fromStdString(desk) + " -> ";
        }
        pathStr.chop(4);  // Remove last " ->"
        pathStr += "\nDistance: " + QString::number(distance);
        ui->textBrowserDisplay->append(pathStr);
    }

    // Clear the input fields
    ui->lineEditSourceDesk->clear();
    ui->lineEditTargetDesk->clear();
}

std::vector<std::string> MainWindow::getDeskNames() {
    std::vector<std::string> deskNames;

    // Iterate through the graph to collect desk names (keys)
    for (const auto& node : graph) {
        deskNames.push_back(node.first);
    }

    return deskNames;
}

std::vector<std::tuple<std::string, std::string, int>> MainWindow::getLinks() {
    std::vector<std::tuple<std::string, std::string, int>> links;

    // Iterate through the graph
    for (const auto& node : graph) {
        const std::string& desk1 = node.first; // First desk name
        for (const auto& edge : node.second) {
            const std::string& desk2 = edge.first; // Second desk name
            int weight = edge.second;             // Weight of the link

            // To avoid duplicates, only add (desk1, desk2, weight) if desk1 < desk2
            if (desk1 < desk2) {
                links.push_back(std::make_tuple(desk1, desk2, weight));
            }
        }
    }

    return links;
}

void MainWindow::on_ResetButton_clicked()
{
    // Clear the graph
    graph.clear();

    // Clear the display area
    ui->textBrowserDisplay->clear();

    // Reset any input fields
    ui->lineEditDeskName->clear();
    ui->lineEditDesk1->clear();
    ui->lineEditDesk2->clear();
    ui->lineEditDistance->clear();
    ui->lineEditSourceDesk->clear();
    ui->lineEditTargetDesk->clear();

    // Show confirmation in the display area
    ui->textBrowserDisplay->append("Graph reset: All desks and links have been deleted.");
}

