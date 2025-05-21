#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>  

// Точка маршрута
struct RoutePoint {
    std::string title;       // Название точки
    double lat;              // Широта
    double lon;              // Долгота
    bool isMandatory;        // Обязательная точка?
    double timePenalty;      // Штраф в часах (если необязательная)
};

// Интерфейс для построения отчётов
class RouteReportBuilder {
public:
    virtual ~RouteReportBuilder() = default;
    virtual void clear() = 0;
    virtual void appendPoint(const RoutePoint& point) = 0;
};

// Построение текстового списка точек
class PointListBuilder : public RouteReportBuilder {
    std::string reportText;
    int counter = 0;

public:
    void clear() override {
        reportText = "";
        counter = 0;
    }

    void appendPoint(const RoutePoint& point) override {
        counter++;
        std::ostringstream stream;
        stream << counter << ") " << point.title << " ("
               << std::fixed << std::setprecision(6)
               << point.lat << ", " << point.lon << ") — ";
        
        if (point.isMandatory) {
            stream << "Обязательный сектор (пропуск = сход)";
        } else {
            stream << "Штраф: " << point.timePenalty << " ч";
        }
        stream << "\n";
        reportText += stream.str();
    }

    std::string getReport() const {
        return reportText;
    }
};

// Подсчёт общего штрафа
class PenaltyCalculator : public RouteReportBuilder {
    double totalPenaltyTime = 0.0;

public:
    void clear() override {
        totalPenaltyTime = 0.0;
    }

    void appendPoint(const RoutePoint& point) override {
        if (!point.isMandatory) {
            totalPenaltyTime += point.timePenalty;
        }
    }

    double calculateTotalPenalty() const {
        return totalPenaltyTime;
    }
};

// Обработчик маршрута
template <typename ReportBuilder>
void generateRouteReport(const std::vector<RoutePoint>& route, ReportBuilder& builder) {
    builder.clear();
    for (const auto& point : route) {
        builder.appendPoint(point);
    }
}

int main() {
    std::vector<RoutePoint> raceRoute = {
        {"Start",        55.755800, 37.617600, true,  0.0},
        {"Intermediate", 55.758000, 37.620000, false, 0.5},
        {"Finish",       55.760000, 37.630000, true,  0.0}
    };

    PointListBuilder listBuilder;
    generateRouteReport(raceRoute, listBuilder);
    std::cout << "Список точек маршрута:\n" << listBuilder.getReport();

    PenaltyCalculator penaltyCalc;
    generateRouteReport(raceRoute, penaltyCalc);
    std::cout << "Общий штраф за необязательные точки: "
              << penaltyCalc.calculateTotalPenalty() << " ч\n";
}