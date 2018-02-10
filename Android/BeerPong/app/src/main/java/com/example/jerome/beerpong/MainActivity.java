package com.example.jerome.beerpong;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import com.example.jerome.beerpong.ColorPicker.ColorPickerDialog;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private Button color ;
    private final static int REQUEST_CODE_ENABLE_BLUETOOTH = 0;
    private Set<BluetoothDevice> pairedDevices, discoveredDevices ;
    BluetoothAdapter bluetoothAdapter ;
    ListView pairedDevicesList, discoveredDevicesList ;

    private final BroadcastReceiver bluetoothReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                discoveredDevices.add((BluetoothDevice) intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE));
                List<String> s = new ArrayList<String>();
                for(BluetoothDevice bt : discoveredDevices)
                    s.add(bt.getName());

                discoveredDevicesList.setAdapter(new ArrayAdapter<String>(getApplicationContext(), android.R.layout.simple_list_item_1, s));
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        pairedDevicesList = (ListView) findViewById(R.id.paired_devices);
        discoveredDevicesList = (ListView) findViewById(R.id.discovered_devices);
        discoveredDevices = new HashSet<BluetoothDevice>();

        color = (Button) findViewById(R.id.color);
        color.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                int initialColor = Color.WHITE;
                ColorPickerDialog colorPickerDialog = new ColorPickerDialog(view.getContext(), initialColor, new ColorPickerDialog.OnColorSelectedListener() {

                    @Override
                    public void onColorSelected(int color) {
                    }

                });
                colorPickerDialog.show();
            }
        });

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter != null){
            if (!bluetoothAdapter.isEnabled()) {
                Intent enableBlueTooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBlueTooth, REQUEST_CODE_ENABLE_BLUETOOTH);
            }
            else{
                bluetooth();
            }
        }
    }

    private void bluetooth(){

        pairedDevices = bluetoothAdapter.getBondedDevices();
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(bluetoothReceiver, filter);
        bluetoothAdapter.startDiscovery();

        List<String> s = new ArrayList<String>();
        for(BluetoothDevice bt : pairedDevices)
            s.add(bt.getName());

        pairedDevicesList.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, s));


    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode != REQUEST_CODE_ENABLE_BLUETOOTH)
            return;
        if (resultCode == RESULT_OK) {
            // L'utilisation a activé le bluetooth
            bluetooth();
        } else {
            // L'utilisation n'a pas activé le bluetooth
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        bluetoothAdapter.cancelDiscovery();
        unregisterReceiver(bluetoothReceiver);
    }

    private class ConnectThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;

        public ConnectThread(BluetoothDevice device) {
            BluetoothSocket tmp = null;
            mmDevice = device;
            try {
                tmp = device.createRfcommSocketToServiceRecord(new UUID(25, 140));
            } catch (IOException e) { }
            mmSocket = tmp;
        }

        public void run() {
            bluetoothAdapter.cancelDiscovery();
            try {
                mmSocket.connect();
            } catch (IOException connectException) {
                try {
                    mmSocket.close();
                } catch (IOException closeException) { }
                return;
            }
            manageConnectedSocket(mmSocket);
        }

        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }

    }

    private void manageConnectedSocket(BluetoothSocket socket){

    }
}
