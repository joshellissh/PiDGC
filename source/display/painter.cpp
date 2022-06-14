#include "painter.h"
#include "converters.h"
#include <QList>
#include <QActionGroup>
#include <QAudioDeviceInfo>
#include <QFontDatabase>

Painter::Painter(VehicleValues &vehicle, Indicators &indicators)
{
    this->vehicle = &vehicle;
    this->indicators = &indicators;

    frameCount = 0;

    backgroundBrush = QBrush(QColor(0, 0, 0));
    shiftLightBrush = QBrush(QColor(255, 0, 0));

    largeGauge.setFamily("BladiTwo4F-Italic");
    largeGauge.setPointSize(30);
    smallGauge.setFamily("BladiTwo4F-Italic");
    smallGauge.setPointSize(15);
    tinyGauge.setFamily("BladiTwo4F-Italic");
    tinyGauge.setPointSize(12);
    normalText.setFamily("Arial");
    normalText.setPointSize(12);
    largeText.setFamily("Arial");
    largeText.setPointSize(20);
    gaugeTextPen = QPen(Qt::white);

    // Load all gauge cluster images
    images = loadImages(":/images/");

    // Load fonts
    QFontDatabase::addApplicationFont(":/fonts/arial.ttf");
    QFontDatabase::addApplicationFont(":/fonts/bladi-two-4f-italic.ttf");

    frameTimer.start();
}

void Painter::paint(QPainter *painter, QPaintEvent *event)
{
    if (vehicle->getRpm() >= 6500)
        painter->fillRect(event->rect(), shiftLightBrush);
    else
        painter->fillRect(event->rect(), backgroundBrush);

    // Run initialization sequence on startup
    if (frameCount < 80) initLoop();
    else vehicle->initLoopFinished = true;

    // Set indicators according to vehicle values
    updateIndicators();

    // Draw center lines
    drawCenteredAt(painter, images->at("center_lines.png"), HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT);

    // Left gauge background
    drawCenteredAt(painter, images->at("left_gauge_bg.png"), 249.0, 240.0);

    // Left gauge boost black overlay
    painter->setBrush(backgroundBrush);
    drawTorus(painter, 249, 240, 78, 125, degToRad(boostToAngle(vehicle->getBoost()) + 91.5f), degToRad(303), 365);

    // Left gauge foreground
    drawCenteredAt(painter, images->at("left_gauge_fg.png"), 249.0, 240.0);

    // Lagging indicator for maximum boost
    if (indicators->boostLaggingMax && vehicle->getBoostLaggingMax() != 0) {
        painter->setOpacity(indicators->boostLaggingMaxOpacity);
        drawCenteredWithOffsetAndRotation(painter, images->at("lagging_max_boost.png"), 249.0, 240.0, -99.0, 0.0, boostToAngle(vehicle->getBoostLaggingMax()));

        drawCenteredAt(painter, images->at("boost_lagging_max_box.png"), 249.0, 172.5);

        painter->setPen(gaugeTextPen);
        QString laggingMax;
        laggingMax.sprintf("%d", (int)vehicle->getBoostLaggingMax());
        painter->setFont(tinyGauge);
        painter->drawText(249.0f - 20.0f, 172.5f - 10.0f, 40.0f, 20.0f, Qt::AlignCenter | Qt::AlignHCenter, laggingMax);
        painter->setOpacity(1);
    }

    // Boost highlight leader
    if (vehicle->getBoost() > 0)
        drawCenteredWithOffsetAndRotation(painter, images->at("blue_hilite_leading.png"), 249.0, 240.0, -99.0, 0.0, boostToAngle(vehicle->getBoost()));

    // Right gauge green fuel background
    if (vehicle->getFuel() > 0.25f)
        drawCenteredAt(painter, images->at("right_gauge_bg.png"), 1031.0, 240.0);

    // Right gauge red fuel background
    if (vehicle->getFuel() <= 0.25f)
        drawCenteredAt(painter, images->at("right_gauge_bg_2.png"), 1031.0, 240.0);

    // Fuel gauge leading (green)
    if (vehicle->getFuel() > 0.25f)
        drawCenteredWithOffsetAndRotation(painter, images->at("green_hilite_leading.png"), 1031.0, 240.0, -99.0, 0.0, fuelToAngle(vehicle->getFuel()));

    // Fuel gauge leading (red)
    if (vehicle->getFuel() <= 0.25f)
        drawCenteredWithOffsetAndRotation(painter, images->at("red_hilite_leading.png"), 1031.0, 240.0, -99.0, 0.0, fuelToAngle(vehicle->getFuel()));

    // Right gauge fuel black overlay
    drawTorus(painter, 1031, 240, 78, 125, degToRad(fuelToAngle(vehicle->getFuel()) + 91.5f), degToRad(303.0f), 365);

    // Right gauge foreground
    drawCenteredAt(painter, images->at("right_gauge_fg.png"), 1031.0, 240.0);

    // Left gauge needle
    drawCenteredWithOffsetAndRotation(painter, images->at("big_needle.png"), 249.0, 240.0, -163.0, 0.0, rpmToAngle(vehicle->getRpm()));

    // Right gauge needle
    drawCenteredWithOffsetAndRotation(painter, images->at("big_needle.png"), 1031.0, 240.0, -163.0, 0.0, mphToAngle(vehicle->getMph()));

    // Coolant gauge needle
    drawCenteredWithOffsetAndRotation(painter, images->at("little_needle.png"), 249.0, 240.0, -191.0, 0.0, temperatureToAngle(vehicle->getCoolant()));

    // Oil gauge needle
    drawCenteredWithOffsetAndRotation(painter, images->at("little_needle.png"), 1031.0, 240.0, -191.0, 0.0, pressureToAngle(vehicle->getOilPressure()));

    // Center logo
    painter->setOpacity(0.8f);
    drawCenteredAt(painter, images->at("center_logo.png"), 640.0, 240.0);
    painter->setOpacity(1.0f);

    // Left blinker indicator
    if (vehicle->getLeftBlinker())
        drawCenteredAt(painter, images->at("left_indicator.png"), 490.0, 115.0);

    // Right blinker indicator
    if (vehicle->getRightBlinker())
        drawCenteredAt(painter, images->at("right_indicator.png"), 790.0, 115.0);

    // Low beam indicator
    if (vehicle->getLowBeam() || vehicle->getHighBeam())
        drawCenteredAt(painter, images->at("low_beam.png"), 565.0, 115.0);

    // Hight beam indicator
    if (vehicle->getHighBeam())
        drawCenteredAt(painter, images->at("high_beam.png"), 565.0, 115.0);

    // Check engine indicator
    if (vehicle->getMil())
        drawCenteredAt(painter, images->at("mil.png"), 640.0, 115.0);

    // Gauge lights/parking lights indicator
    if (vehicle->getGaugeLights())
        drawCenteredAt(painter, images->at("parking-lights.png"), 715.0, 115.0);

    // Oil indicator
    if (indicators->oil)
        drawCenteredAt(painter, images->at("oil.png"), 603.0, 362.0);

    // Battery indicator
    if (indicators->battery)
        drawCenteredAt(painter, images->at("battery.png"), 678.0, 362.0);

    // Low fuel indicator
    if (indicators->fuel)
        drawCenteredAt(painter, images->at("fuel.png"), 736.0, 362.0);

    // Coolant temp indicator
    if (indicators->coolant)
        drawCenteredAt(painter, images->at("coolant.png"), 536.0, 362.0);

    // Reset trip icon
    painter->setOpacity(0.8);
    drawCenteredAt(painter, images->at("reset.png"), 410.0, 423.0);
    painter->setOpacity(1.0);

    // Microcontroller icon
    if (vehicle->getSerialConnected())
        painter->setOpacity(0.8);
    else
        painter->setOpacity(0.3);
    drawCenteredAt(painter, images->at("microcontroller.png"), 1240.0, 33.0);
    painter->setOpacity(1.0);

    painter->setPen(gaugeTextPen);
    painter->setFont(largeGauge);
    // RPM
    painter->drawText(247.0f - 60.0f, 235.0f - 20.0f, 120.0f, 40.0f, Qt::AlignCenter | Qt::AlignHCenter, QString("%1").arg(vehicle->getRpm()));
    // MPH
    painter->drawText(1029.0f - 60.0f, 235.0f - 20.0f, 120.0f, 40.0f, Qt::AlignCenter | Qt::AlignHCenter, QString("%1").arg((int)vehicle->getMph()));
    // Boost
    painter->setFont(smallGauge);
    painter->drawText(247.0f - 60.0f, 340.0f - 10.0f, 120.0f, 20.0f, Qt::AlignCenter | Qt::AlignHCenter, QString("%1 psi").arg((int)vehicle->getBoost()));
    // Battery voltage
    QString voltage;
    voltage.sprintf("%.1f", vehicle->getVoltage());
    painter->setFont(smallGauge);
    painter->drawText(1031.0f - 20.0f, 388.5f - 10.0f, 40.0f, 20.0f, Qt::AlignCenter | Qt::AlignHCenter, voltage);
    // Coolant temp
    QString temperature;
    temperature.sprintf("%d", (int)vehicle->getCoolant());
    painter->setFont(tinyGauge);
    painter->drawText(249.0f - 20.0f, 390.5f - 10.0f, 40.0f, 20.0f, Qt::AlignCenter | Qt::AlignHCenter, temperature);

    painter->setFont(normalText);
    painter->setOpacity(0.8);
    // Trip odometer
    QString tripText;
    tripText.sprintf("Trip: %.1f", vehicle->getTripOdometer());
    painter->drawText(430.0f, 415.0f, 300.0f, 20.0f, Qt::AlignLeading, tripText);

    // Odometer
    QString odometerText;
    odometerText.sprintf("%.1f", vehicle->getOdometer());
    painter->drawText(575.0f, 415.0f, 300.0f, 20.0f, Qt::AlignTrailing, odometerText);

    // Dimmer for when lights are on (night time)
    if (vehicle->getGaugeLights()) {
        painter->setOpacity((float)vehicle->config.getScreenDimming() * 0.01f);
        painter->fillRect(event->rect(), backgroundBrush);
        painter->setOpacity(1);
    }

    // Frame counter
    frameCount++;
}

// Startup animation
void Painter::initLoop() {
    if (frameCount < 40) {
        vehicle->setRpm(vehicle->getRpm() + 200);
        vehicle->setMph(vehicle->getMph() + 4);
        vehicle->setCoolant(vehicle->getCoolant() + 6.25);
        vehicle->setFuel(vehicle->getFuel() + 0.025);
        vehicle->setBoost(vehicle->getBoost() + 0.8);
        vehicle->setVoltage(vehicle->getVoltage() + 0.375);
        vehicle->setOilPressure(vehicle->getOilPressure() + 2.25);

        vehicle->setLeftBlinker(true);
        vehicle->setRightBlinker(true);
        vehicle->setLowBeam(true);
        vehicle->setHighBeam(true);
        vehicle->setGaugeLights(true);
        vehicle->setMil(true);

        indicators->battery = true;
        indicators->coolant = true;
        indicators->fuel = true;
        indicators->oil = true;
    }
    if (frameCount >= 40) {
        vehicle->setRpm(vehicle->getRpm() - 200);
        vehicle->setMph(vehicle->getMph() - 4);
        vehicle->setCoolant(vehicle->getCoolant() - 6.25);
        vehicle->setFuel(vehicle->getFuel() - 0.025);
        vehicle->setBoost(vehicle->getBoost() - 0.8);
        vehicle->setVoltage(vehicle->getVoltage() - 0.375);
        vehicle->setOilPressure(vehicle->getOilPressure() - 2.25);

        vehicle->setLeftBlinker(false);
        vehicle->setRightBlinker(false);
        vehicle->setLowBeam(false);
        vehicle->setHighBeam(false);
        vehicle->setGaugeLights(false);
        vehicle->setMil(false);

        indicators->battery = false;
        indicators->coolant = false;
        indicators->fuel = false;
        indicators->oil = false;
    }
}

void Painter::updateIndicators() {
    // Boost lagging max
    if (vehicle->getBoost() > vehicle->getBoostLaggingMax() && vehicle->getBoost() > 0.0 && frameCount > 80) {
        vehicle->setBoostLaggingMax(vehicle->getBoost());
        indicators->boostLaggingMax = false;
    }

    if (vehicle->getBoost() < vehicle->getBoostLaggingMax()) {
        if (!indicators->boostLaggingMax) {
            indicators->boostLaggingMax = true;
            indicators->boostLaggingMaxOpacity = 1.0f;
        }

        indicators->boostLaggingMaxOpacity -= 0.005;

        if (indicators->boostLaggingMaxOpacity <= 0) {
            indicators->boostLaggingMaxOpacity = 0;
            vehicle->setBoostLaggingMax(0);
        }
    }
}
