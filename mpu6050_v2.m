%% Initialize Arduino
hold all
a = arduino; %Sets up arduino connection
mpu=i2cdev(a,'0x68'); %mpu adress is normally 0x68, i2c communication with sensor
writeRegister(mpu, hex2dec('B6'), hex2dec('00'), 'int16'); %reset
data=zeros(10000,14,'int8'); %prelocating for the speed

% To find sensitivity values, go to: https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/
gyro_sens = 131; %Defaulted sensitivity for gyroscope full scale range
accel_sens = 16384; %Defaulted sesnsitivity for accelerometer full scale range

% Sets up live plot to detect accel and gyro changes
j=1;
figure(1)
a1 = animatedline('Color',[1 0 0]); 
a2 = animatedline('Color',[0 1 0]);
a3 = animatedline('Color',[0 0 1]);
legend('Accel_x','Accel_y','Accel_z')

figure(2)
g1 = animatedline('Color',[1 0 0]); 
g2 = animatedline('Color',[0 1 0]);
g3 = animatedline('Color',[0 0 1]);
legend('Gyro_x','Gyro_y','Gyro_z')

%% Register Access Loop (NEED TO CHANGE SO IT CAN BE MANUALLY STOPPED)
while(true)
    x=1;
    for i=59:72 % 14 Data Registers for Accel,Temp,Gyro
        data(j,x)= readRegister(mpu, i, 'int8'); % Note: This data is still in LSB format, must convert using sensitivity values above
        x=x+1;
    end
    y=swapbytes(typecast(data(j,:), 'int16')) %if your system is big-endian remove the swapbytes function
    addpoints(a1,j,double(y(1))/accel_sens);
    addpoints(a2,j,double(y(2))/accel_sens);
    addpoints(a3,j,double(y(3))/accel_sens);
    
    addpoints(g1,j,double(y(5))/gyro_sens);
    addpoints(g2,j,double(y(6))/gyro_sens);
    addpoints(g3,j,double(y(7))/gyro_sens);
   
    j=j+1;
    drawnow limitrate
end
    