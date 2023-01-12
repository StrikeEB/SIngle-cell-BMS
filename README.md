### Table of Contents

* [Project motivation](#Project-motivation)
* [Prototype design, architecture and scope](Prototype-design,-architecture-and-scope)
* [Methodology](Methodology)
* [Voltage sensor measuring battery discharge](Voltage-sensor-measuring-battery-discharge)
* [Measuring cell’s temperature](Measuring-cell’s-temperature)
* [Measuring current flow](Measuring-current-flow)
* [Automation, analysis and insight visualisation](Automation,-analysis-and-insight-visualisation)
* [Learnings](Learnings)
* [Prototype benefits](Prototype-benefits)
* [Key areas for further development](Key-areas-for-further-development)
* [Controls](Controls)
* [Precision](Precision)
* [System size and efficiency](System-size-and-efficiency)
* [Multi-cell BMS](Multi-cell-BMS)
* [References](References)


## Project motivation

Batteries are an enabler of the renewable future. However, if not looked after, they might cause fires, explosions or simply not deliver the crucial services required (Ma et al, 2018; Pattipati et al, 2011). Even with the current technology, battery performance is difficult to predict (Ungurean et al, 2016). Better understanding and innovation in the field will help to increase safety, reliability and affordability of this crucial technology. 

Building a single-cell Battery Monitoring System (“BMS”) from scratch is a good way to learn about the cell-level performance and pin-point challenges under different battery use cases at this level. The prototype has a scalable Internet-of-Things (“IoT”) architecture and could be expanded horizontally to monitor multiple cells simultaneously or vertically to monitor a multi-cell rack.
## Prototype design, architecture and scope 

The prototype ecosystem consists of a single-cell Li-Po battery, a load to discharge it and a charger. The battery performance evaluation when discharging is executed remotely.

![Alt text]( https://github.com/StrikeEB/Single-cell-BMS/blob/main/Figure1_The%20prototype%20design.jpg)

The prototype’s IoT architecture is mostly based on the stack proposed by Prof. Wilson for the Plant Monitoring project (Wilson, n.d.), but Grafana is replaced with PowerBI. The architecture consists of a single slave node and a master node/server, which both communicate to the data broker remotely, which transmit data to InfluxDB database via Telegraf. Data extraction from the database, analytics and insight visualisation are automated in PowerBI. 

![Alt text]( https://github.com/StrikeEB/Single-cell-BMS/blob/main/Figure%202_IoT%20Architecture.jpg)

This prototype utilises Arduino Elegoo and Adafruit learning tools. Full list of parts is shared in the GitHub repository (Butkute, 2023). 
The prototype is sensing only and the controls therefore are done manually. The current design requires Li-Po to be charged-up using a charger and then plugged in to the BMS to measure voltage, current flow and temperature as it’s discharging. These three measurements allow to calculate a number of Key Performance Indicators (“KPIs”) and calculated variables to monitor operational activity (NPTEL, 2020). 

![Alt text]( https://github.com/StrikeEB/Single-cell-BMS/blob/main/Figure%203_Sensed%20data%20points%20and%20possible%20outputs.jpg)

## Methodology 

A top-down and reiterative approach was used to creating design and build the BMS prototype:

![Alt text]( https://github.com/StrikeEB/Single-cell-BMS/blob/main/Figure%204_Approach%20to%20creating%20design%20and%20building%20BMS%20prototype.jpg)

First, a scientific literature and industry research of the problem, i.e. battery performance, was done to develop a vision for the tool. The subsequent design process was in particular influenced by the lecture series on the fundamentals of battery technology and economics led by Prof. Jhunjhunwala (NPTEL, 2020).

Existing prototypes and reports addressing similar challenges were also researched. Two most relevant prototypes discovered were developed by the online community Hackster’s member Katarinak998 (Katarinak998, 2021) and Adafruit website contributor Liz Clark (Clark, 2022). 

A common shortcoming of similar BMS prototypes found online was lack of scalable IoT architecture. Solution delivered by this prototype is based on the IoT architecture of a Plant Monitoring system developed by  Prof. Wilson (Wilson, n.d.). Some parts of the stack were changed to meet specific vision needs.

Then, the available tools and their features were studied. In particular,  factsheets (J&A, 2017), Elegoo kit tutorials (Elegoo, 2020) and Elegoo tutorials GitHub repository developed by Richardo Moreno (Moreno, 2020) helped to understand what parts can be used, their capabilities, limitations and also compatibility with other parts, software and libraries. Methodology of key design features and journey of defining them is discussed below. Learnings are discussed in section 4.
 
# Voltage sensor measuring battery discharge
Voltage is measured by dividing the input voltage against two resistors as described in an online article by EG Projects (EG Projects, n.d.).  


![Alt text](https://github.com/StrikeEB/Single-cell-BMS/blob/main/Figure%205_Voltage%20calculation%20(EG%20projects).jpg)

Ideally, reference voltage and resistance R1 and R2  would be measured by testing a specific battery multiple times, but due to time constraints they were taken from the DroneBot Workshop (DroneBot, 2021) and integrated with the LCD and fan motor code proposed by Elegoo (Elegoo, 2020; Moreno, 2020). Full code is available in the dedicated GitHub repository (Butkute, 2023). 

https://github.com/StrikeEB/Single-cell-BMS/blob/5f5b5132ea5286ed6d1b0b426c6343dd84a4ec2d/BMS%20code#L148-L158

To ensure that Li-Po and not Arduino is powering the motor, a power supply module was used (Elegoo, 2020, p. 199). It was proven that this method works as when Li-Po is disconnected the motor stops spinning. Voltage is measured between the Li-Po and the motor.
Note, LCD connection also requires a separate breadboard so that it would be powered by Arduino as Li-Po is not powerful enough to drive both the motor and the screen.

![Alt text]( https://github.com/StrikeEB/Single-cell-BMS/blob/main/Figure%206_Voltage%20discharge%20measurement%20wiring.jpg)

Time-series observation proved that the set-up is working as the battery was discharging the longer it powered the fan motor and hence the voltage was decreasing as expected . 

![Alt text](https://github.com/StrikeEB/Single-cell-BMS/blob/main/Figure%207_%20Voltage%20observation%20as%20Li-Po%20is%20discharging.jpg)


# Measuring cell’s temperature

The temperature sensor LM35 was used. Franklin Electric Grid Solutions tutorial explains that the sensor needs to be as close as possible to the battery cell to ensure that the cell’s temperature is reflected rather than the room’s (Franklin Electric Grid Solutions, 2015).

![Alt text]( https://github.com/StrikeEB/Single-cell-BMS/blob/main/Figure%208_Temperature%20sensor%20wiring.jpg)


The code and wiring methodology was taken from the Arduino temperature sensor tutorial (Tutorials Point, n.d.) and added to the voltage set up shown above by adding an additional void function do_temp. 

https://github.com/StrikeEB/Single-cell-BMS/blob/5f5b5132ea5286ed6d1b0b426c6343dd84a4ec2d/BMS%20code#L160-L171

# Measuring current flow

The current is measured with AZ Delivery ACS712 30A sensor. The datasheet (Allegro, n.d.) and Robojax tutorial were used for code development (Robojax, 2020). To measure the current, Robojax_AllegroACS_Current_Sensor.h library has to be added. It’s also important to choose the correct model type as explained on the Robojax website (Robojex, 2020).

https://github.com/StrikeEB/Single-cell-BMS/blob/5f5b5132ea5286ed6d1b0b426c6343dd84a4ec2d/BMS%20code#L94-L97


Remaining code is similar to temperature and voltage. An additional do_current void function needs to be added: 


https://github.com/StrikeEB/Single-cell-BMS/blob/5f5b5132ea5286ed6d1b0b426c6343dd84a4ec2d/BMS%20code#L173-L183

The code was then integrated with the main temperature and voltage code. Robojax tutorial also shows how to connect the sensor to the load and battery (Robojax, 2020):

![Alt text]( https://github.com/StrikeEB/Single-cell-BMS/blob/main/Figure%209_Current%20sensor%20wiring.jpg)
 
# Automation, analysis and insight visualisation 

It’s possible to completely automate sensing, analysis and insight visualisation when the battery is discharging  using the proposed IoT stack . The data node consists of Arduino Mega which is capable of reading multiple analogue inputs, three sensors and an ESP32. ESP32 enables data delivery to MQTT via an inbuilt Wi-Fi  module. How to connect ESP32 to Arduino Uno which is very similar to Mega is explained in a video tutorial by Circuit Desolator (Circuit Desolator, 2020). An example code how to connect  ESP32 to MQTT can be taken from the Plant-Monitor workshop (Wilson, n.d.).

Next, the prototype requires a server where to download Telegraf, set up InfluxDB and PowerBI. For this a RaspberryPi computer was used. The Plant-Monitor workshop was also followed to set up Telegraf and InfluxDB (Wilson, n.d.). PowerBI-Desktop can be downloaded for free from Microsoft website (Microsoft, n.d.). The App uses M and Dax languages. At the moment, it supports direct connection to SQL databases but not InfluxDB . However, this can be overcome in multiple ways and the data flow from InfluxDB to PowerBI can still be automated. For example, the connection can be created using OData Rest API as discussed in the Synergetic Engineering GitHub (Synergetic Engineering, 2019).

The industry prefers SQL rather than InfluxDB because SQL is a relational database and can store broad range of data about the business, operations and products that can be combined with time-series data from sensors.  A good example is Spec-India’s BMS (Spec-India, n.d.). However, InfluxDB is a dedicated time-series repository that benefits users who require close to real-time data.  

PowerBI benefits include ability to automate the data pull, transform source data, combine it with other data, produce calculated variables, run statistical analysis, use simple Machine-Learning tools and produce interactive insights by dragging-and-dropping. Dashboards need to be created once and can be shared with multiple users yet remain controlled by the developer/owner. Access level to different users can be controlled allowing the developer to control who sees what level of sensitive data. Data refresh can be automated using scheduled refreshes functionality that works via PowerBI gateways. The free version covers all of these functions but only for a limited number of viewers (PowerBI, n.d. b) and training is also accessible for free (Microsoft Learn, n.d.). 
 
## Learnings

# Prototype benefits

The key prototype’s goal was to contribute to the existing BMS resources available online by introducing a scalable IoT architecture to contribute towards accessible learning about battery performance and resulting innovation. The stack used is free of charge and easy to use even for less technical people. Additional data nodes could be added to monitor multiple cells simultaneously and compare performance. By adjusting measurement parameters (such as R1 and R2 for voltage), different chemistry cells could also be monitored. Finally, the prototype can also be easily replicated with minimal budget.

# Key areas for further development 

# Controls 

Next step for the BMS would be to create a controlled environment, including control voltage, current, speed of charging/discharging and temperature of the battery. For example, for future design iterations,  Amplify by Jilbee could be followed to consider adding a diode and a swich to the L293D motor driver module (Amplify by Jilbee, 2021). The control over current supplied to or taken from the battery would still be limited to the speed of the motor setting rather than controlled in Amps. This however could be overcome by estimating how many Amps need to be supplied for the blade motor to spin for 1 min at various speeds and although the outcome would not be continuous, a good initial estimate could be achieved.  

![Alt text]( https://github.com/StrikeEB/Single-cell-BMS/blob/main/Figure%2010_Bi-directional%20current%20flow%20(Amplify%20by%20Jilbee%2C%202021).jpg)

Also, an integrated heating and cooling systems are widely used in more sophisticated systems (Ludre, 2022) and could potentially be integrated in future iterations of this prototype. These small systems are widely available in the UK market and could prevent over-heating and  reduce risk of thermal runaway.

# Precision 

Inability to control volume and speed of current and voltage injections creates bias when calculating KPIs. For example, variance in cell voltage depends on chemistry, pressure, temperature and other variables (Stuart and Zhu, 2011) and single-cell LiPo batteries average 3.7V (Nagel, 2021). The experiment showed that when charged the battery was around 4.2V-4.3V and when unable to power the blade motor anymore the voltage dropped to ~ 2.7V. However, if the voltage is not 0, then it means that the battery hasn’t been discharged 100%. Sensing when discharging but not sensing when charging up also adds uncertainty to the real battery’s SoH (NPTEL, 2020).

# System size and efficiency 

This prototype system utilises predesigned tools. Examples of manufacturer design challenges were faced when building this prototype, including connecting the battery to the motor without powering Arduino Mega, connecting an LCD1602 screen without drawing power from the battery and connecting multiple sensors simultaneously when the battery’s manufacturer designed a 2-pin connector needs to be plugged in via a power supply module to power the blade motor. Solutions involved additional parts and a separate breadboard, which made the wiring complicated and hence the prototype’s design chunky. Further design iterations would benefit from more powerful and smaller parts. 

# Multi-cell BMS

Finally, the  literature review showed that battery racks perform very differently than cells depending how they’re connected and operated (NPTEL, 2020). Most battery applications contains battery racks with different cell connection techniques, including EVs, ships, commercial batteries and other. Hence, further design iterations to consider multi-cell applications would be beneficial. 
 
## References

Amplify by Jilbee. (2021). 2-Quadrant Bi-directional Current Switch [Video file]. Retrieved from https://www.youtube.com/watch?v=nrCvXzA2s9k&ab_channel=AMPLIFYbyJilbee

Allegro. (n.d.). Fully integrated, hall effect-based linear current sensor with 2.1 kVRMS voltage isolation and a low-resistance current conductor. Retrieved from https://www.sparkfun.com/datasheets/BreakoutBoards/0712.pdf


Clark, L. (2022). Adafruit MAX17048 LiPoly / LiIon Fuel Gauge and Battery Monitor. Retrieved from https://learn.adafruit.com/adafruit-max17048-lipoly-liion-fuel-gauge-and-battery-monitor/python-circuitpython

Circuit Desolator. (2020, October 11). How to connect ESP32 to Arduino Uno Step by Step [Video file]. Retrieved from https://www.youtube.com/watch?v=YVPumD16Y_Y&ab_channel=CircuitDesolator

DroneBot. (2021). Measure DC Voltage and Current with an Arduino. Retrieved from https://dronebotworkshop.com/dc-volt-current/

Elegoo. (2020). Tutorial for MEGA2560 [PDF file]. Retrieved from https://images-na.ssl-images-amazon.com/images/I/D1oC-c3G5TS.pdf

EG Projects. (n.d.). How to measure voltage of multiple batteries connected in string/array in series or parallel combination with microcontrollers. Retrieved from https://www.engineersgarage.com/series-combination-of-batteries-voltage-monitor/

Franklin Electric Grid Solutions. (2015, September 16). Taking a battery temperature measurement with the Midtronics Celltron Advantage Battery Analyzer [Video file]. Retrieved from https://www.youtube.com/watch?v=_vdpkPuHbDs&ab_channel=FranklinElectricGridSolutions

J&A. (2017). Lithium-ion polymer battery specification [PDF file]. Retrieved from https://www.olimex.com/Products/Power/Lipo-battery/BATTERY-LIPO800mAh/resources/JA603048-Spec-Data-Sheet-3.7V-800mAh--170116.pdf

Katarinak998. (2021). Li-Ion battery measurement system. [Project]. Retrieved from https://www.hackster.io/katarinak998/li-ion-battery-measurement-system-f3236e

Ludre. (2022). Battery management system [PDF file]. Retrieved from https://www.ludre.com.tr/Uploads/Dokumanlar/battery-management-system-eng-bms9-agustos-2022-sali.pdf

Ma, M., Wang, Y., Duan, Q., Wu, T., Sun, J., & Wang, Q. (2018). Fault detection of the connection of lithium-ion power batteries in series for electric vehicles based on current-voltage characteristics and the Kalman filter. Energy, 150, 193-204.

Microsoft. (n.d.). Download PowerBI tools and apps. Retrieved from https://powerbi.microsoft.com/en-us/downloads/

Microsoft Learn. (n.d.). Power BI developer documentation. Retrieved from https://learn.microsoft.com/en-us/power-bi/developer/

Moreno, R. (2020). Elegoo tutorial. [Code files]. Retrieved from https://github.com/rmorenojr/ElegooTutorial

Nagel, L. (2021). A guide to lithium polymer batteries for drones. Retrieved from https://www.tytorobotics.com/blogs/articles/a-guide-to-lithium-polymer-batteries-for-drones#:~:text=For%20common%20LiPo%20batteries%2C%20the,most%20of%20the%20battery%20life

NPTEL. (2020). Fundamentals of Electric vehicles: Technology & Economics. [Online course]. Retrieved from https://onlinecourses.nptel.ac.in/noc20_ee99/preview

Pattipati, B., Sankavaram, C., & Pattipati, K. (2011). System identification and estimation framework for pivotal automotive battery management system characteristics. IEEE Transactions on Systems, Man, and Cybernetics, Part C (Applications and Reviews), 41(6), 869-884.

PowerBI. (n.d.a). Open PowerBI to InfluxDB databases. Retrieved from https://ideas.powerbi.com/ideas/idea/?ideaid=8afc47cf-dbfe-4bc7-84bb-54c93927fcbf

PowerBI. (n.d.b). Getting started with Power BI. Retrieved from https://powerbi.microsoft.com/en-au/getting-started-with-power-bi/?accordion=getting-started-with-powerbi-accordion&panel=p1&tab=t1

Spec-India. (n.d.). Power BI Dashboard and Data Analytics for Battery Manufacturer. Retrieved from https://www.spec-india.com/case-study/power-bi-dashboard-and-data-analytics-battery-manufacturer

Stuart, T.A., & Zhu, W. (2011). Modularized battery management for large lithium ion cells. Journal of Power Sources, 196(1), 458-464.

Synergetic Engineering. (2019). influxdb_odata [Code files]. Retrieved from https://github.com/Synergetic-Engineering/odata-influxdb

Ungurean, L., Cârstoiu, G., Micea, M.V., & Groza, V. (2017). Battery state of health estimation: A structured review of models, methods and commercial devices. International Journal of Energy Research, 41(2), 151-181.

Wilson, D. (n.d.). Plant-Monitor. Retrieved from https://workshops.cetools.org/casa0014/
