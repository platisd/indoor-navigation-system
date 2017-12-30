package ins.developers.indoornavigationsystem;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {
    final String TAG = this.getClass().getSimpleName();
    TextView responseText;
    Button getRequestButton;
    EditText employeeSelector;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        responseText = (TextView) findViewById(R.id.result);
        getRequestButton = (Button) findViewById(R.id.get_button);
        employeeSelector = (EditText) findViewById(R.id.node_setter);

        getRequestButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Instantiate the RequestQueue.
                RequestQueue queue = Volley.newRequestQueue(getApplicationContext());
                String selectedEmployee = employeeSelector.getText().toString();
                String employeeId = selectedEmployee.length() != 0 ? selectedEmployee : "1"; // Default employee id
                String url ="http://10.0.2.2:8050/get_device_pos/" + employeeId; //TO-DO: Change to get_employee_pos

                // Request a string response from the provided URL.
                JsonObjectRequest stringRequest = new JsonObjectRequest(Request.Method.GET, url,
                        new Response.Listener<JSONObject>() {
                            @Override
                            public void onResponse(JSONObject response) {
                                try {
                                    String xVal = response.getString("pos_x");
                                    String yVal = response.getString("pos_y");
                                    String zVal = response.getString("pos_z");
                                    responseText.setText("X:" + xVal + " Y:" + yVal + " Z:" + zVal);
                                } catch (JSONException e) {
                                    responseText.setText("Error parsing server response!");
                                    e.printStackTrace();
                                }
                            }
                        }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.e(TAG, error.toString());
                        responseText.setText("Error, employee not found!");
                    }
                });
                // Add the request to the RequestQueue.
                queue.add(stringRequest);
            }
        });
    }
}
