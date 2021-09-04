#include "./Headers/chartwindow.h"

chartwindow::chartwindow(QWidget *parent, QString title, int row) :
    QWidget(parent),
    ui(new Ui::chartwindow)
{
    ui->setupUi(this);
    QLineSeries *series = new QLineSeries();

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->setTitle(title);


    QPen pen(QRgb(0x000000));
    pen.setWidth(5);
    series->setPen(pen);


    stockdata *sd = stockdata::getInstance();
    QVector<QVector<dataframe>> *asset_values = sd->getDataframes();
    //Access error not checked
    QCategoryAxis *axisX = new QCategoryAxis();
    for(int c = 0; c < asset_values->at(row).size(); c++) {
        float a = asset_values->at(row).at(c).close_price;
        series->append(c, a);
    }
    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%.2f");
    axisY->setTitleText("USD");
    /*
    series->append(0, 1);
    series->append(1, 2);
    series->append(2, 3);
    series->append(3, 4);
    series->append(4, 5);
    series->append(5, 6);
    series->append(6, 7);*/
    //chart->axes(Qt::Vertical).first()->setRange(-500, 3000);

    for(int c = 0; c < asset_values->at(row).size(); c++) {
        QString b = asset_values->at(row).at(c).time;
        axisX->append(b, c);
    }
    axisX->setTitleText("Date");
    axisX->setTitleVisible(true);
    axisX->setTickCount(10);
    //QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(QChart::ChartThemeQt)>QChart::ChartThemeQt;
    /*
    QDateTimeAxis *axisX_time = new QDateTimeAxis;
    axisX_time->setTickCount(10);
    axisX_time->setFormat("MMM yyyy");
    axisX_time->setTitleText("Date");
    chart->addAxis(axisX_time, Qt::AlignBottom);
    series->attachAxis(axisX);*/


    chart->createDefaultAxes();
    chart->addSeries(series);
    chart->addAxis(axisY, Qt::AlignBottom);
    chart->setAxisX(axisX, series);
    chart->legend()->show();
    chart->setAnimationOptions(QChart::AllAnimations);
    chartView = new QChartView(chart, ui->chart_widget);
    chartView->setRubberBand(QChartView::HorizontalRubberBand);
    chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(ui->chart_widget->size());
}

chartwindow::~chartwindow()
{
    delete ui;
}
