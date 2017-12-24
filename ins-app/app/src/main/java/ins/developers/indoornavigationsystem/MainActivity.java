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
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

public class MainActivity extends AppCompatActivity {
    final String TAG = "InsMain";
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
                String employeeId = selectedEmployee.length() != 0 ? selectedEmployee : "1"; // Default nodeId
                String url ="http://10.0.2.2:5000/node/" + employeeId;

                // Request a string response from the provided URL.
                StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                        new Response.Listener<String>() {
                            @Override
                            public void onResponse(String response) {
                                responseText.setText("Response is: " + response);
                            }
                        }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.e(TAG, error.toString());
                        responseText.setText("Error");
                    }
                });
                // Add the request to the RequestQueue.
                queue.add(stringRequest);
            }
        });
    }
}
