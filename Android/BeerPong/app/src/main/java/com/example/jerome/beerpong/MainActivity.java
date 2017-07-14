package com.example.jerome.beerpong;

import android.graphics.Color;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

import com.example.jerome.beerpong.ColorPicker.ColorPickerDialog;

public class MainActivity extends AppCompatActivity {

    private Button color ;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

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

    }
}
