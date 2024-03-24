module com.example.modelrepairman {
    requires javafx.controls;
    requires javafx.fxml;


    opens com.example.modelrepairman to javafx.fxml;
    exports com.example.modelrepairman;
}