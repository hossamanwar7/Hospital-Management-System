  #include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <unordered_map>
using namespace std;

// Simple hash for password (not secure in real world)
string simpleHash(int pass) {
    return to_string(pass * 123456 % 1000000);
}

// ======== CLASS PERSON ========
class person {
protected:
    int id;
    string name;
    string gender;
    string address;
    long long phone;
    int age;
    static int count;

public:
    person() {}
    person(string n, string g, string ad, long long ph, int ag) {
        count++;
        id = count;
        name = n;
        gender = g;
        address = ad;
        phone = ph;
        age = ag;
    }

    int get_id() const { return id; }
    string get_name() const { return name; }

    void display() const {
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Gender: " << gender << endl;
        cout << "Address: " << address << endl;
        cout << "Phone: " << phone << endl;
        cout << "Age: " << age << endl;
    }
};
int person::count = 0;

// ======== CLASS USER ========
class user {
protected:
    string username;
    string password;
    string role;

public:
    user() {}
    user(string u, int p, string r) {
        username = u;
        password = simpleHash(p);
        role = r;
    }

    bool login(int p) {
        return password == simpleHash(p);
    }

    string get_username() const { return username; }
    string get_role() const { return role; }
};

// ======== CLASS PATIENT ========
class patient : public person, public user {
private:
    int patientid;
    string disease;
    string prescription;
    int medicalhistory;
    static int count;

public:
    patient() {}
    patient(string n, string g, string ad, long long ph, int ag,
            string u, int p, string r, string di, string pr, int mh)
        : user(u, p, r), person(n, g, ad, ph, ag)
    {
        count++;
        patientid = count;
        disease = di;
        prescription = pr;
        medicalhistory = mh;
    }

    int getPatientId() const { return patientid; }
    string getpatientName() { return get_name(); }
    string getDisease() { return disease; }
    void setPrescription(string pr) { prescription = pr; }
    string getPrescription() { return prescription; }
    void display() {
        cout << "\n=== Patient Info ===\n";
        person::display();
        cout << "Patient ID: " << patientid << endl;
        cout << "Disease: " << disease << endl;
        cout << "Prescription: " << prescription << endl;
    }
};
int patient::count = 0;

// ======== CLASS DOCTOR ========
class doctor : public person, public user {
private:
    string specialization;
    float available_time;
    queue<pair<int, string>> patient_list;
    int doctor_Id;
    static int count;

public:
    doctor() {}
    doctor(string u, int pass, string r,
           string n, string g, string ad, long long ph, int ag,
           string sp, float at)
        : user(u, pass, r), person(n, g, ad, ph, ag)
    {
        count++;
        doctor_Id = count;
        specialization = sp;
        available_time = at;
    }

    string getDoctorName() { return get_name(); }
    string getUsername() { return get_username(); }
    string getSpecialization() { return specialization; }
    int getDoctorId() const { return doctor_Id; }

    void addPatient(int id, const string& name) {
        patient_list.push({id, name});
    }

    void servePatient() {
        if (patient_list.empty()) {
            cout << "No patients waiting.\n";
            return;
        }
        cout << "Serving: " << patient_list.front().second << endl;
        patient_list.pop();
    }

    void showNextPatient() {
        if (patient_list.empty())
            cout << "No patients waiting.\n";
        else
            cout << "Next patient: " << patient_list.front().second << endl;
    }

    void displayDoctor() {
        cout << "\n==== Doctor Info ====\n";
        person::display();
        cout << "Doctor Username: " << getUsername() << endl;
        cout << "Specialization: " << specialization << endl;
        cout << "Available Time: " << available_time << endl;
        cout << "Patients Waiting: " << patient_list.size() << endl;
    }
};
int doctor::count = 0;

// ======== CLASS PRESCRIPTION ========
class prescription {
private:
    int prescriptionId;
    doctor Doctor;
    patient Patient;
    vector<string> medicinesList;
    vector<float> medicinePrices;
    float totalCost;
    static int count;

public:
    prescription() { totalCost = 0; }
    prescription(doctor d, patient p) {
        count++;
        prescriptionId = count;
        Doctor = d;
        Patient = p;
        totalCost = 0;
    }

    void addMedicine(string med, float cost) {
        medicinesList.push_back(med);
        medicinePrices.push_back(cost);
        totalCost += cost;
    }

    float calculateCost() {
        totalCost = 0;
        for (float price : medicinePrices)
            totalCost += price;
        return totalCost;
    }

    void showPrescription() {
        cout << "\n=== Prescription Info ===\n";
        cout << "Prescription ID: " << prescriptionId << endl;
        cout << "Doctor: " << Doctor.getDoctorName() << endl;
        cout << "Patient: " << Patient.getpatientName() << endl;
        cout << "Medicines:\n";
        for (size_t i = 0; i < medicinesList.size(); i++)
            cout << "- " << medicinesList[i] << " (" << medicinePrices[i] << " EGP)\n";
        cout << "Total Cost: " << totalCost << " EGP\n";
    }

    int getPrescriptionId() { return prescriptionId; }
    int getPatientId() { return Patient.getPatientId(); }
};
int prescription::count = 0;

// ======== CLASS PAYMENT ========
class payment {
private:
    int billid;
    patient Patient;
    prescription* Pres;
    float totalAmount;
    string paymentMethod;
    static int count;

public:
    payment() { count++; billid = count; totalAmount = 0.0; paymentMethod = "Pending"; Pres = nullptr; }
    payment(patient P, prescription* pr) { count++; billid = count; Patient = P; Pres = pr; totalAmount = 0.0; paymentMethod = "Pending"; }

    int getBillId() { return billid; }

    void calculateTotal() {
        if (Pres)
            totalAmount = Pres->calculateCost();
        else
            totalAmount = 0;
    }

    void choosePaymentMethod() {
        int c;
        cout << "\nChoose Payment Method:\n1. Card Payment\n2. Bank Transfer\n3. E-Wallet\n4. Cash\nChoice: ";
        cin >> c;
        switch (c) {
        case 1: paymentMethod = "Card Payment"; break;
        case 2: paymentMethod = "Bank Transfer"; break;
        case 3: paymentMethod = "E-Wallet"; break;
        case 4: paymentMethod = "Cash"; break;
        default: paymentMethod = "Invalid"; break;
        }
    }

    void displayBill() {
        cout << "\n======= BILL INFO =======\n";
        cout << "Bill ID: " << billid << endl;
        cout << "Patient Name: " << Patient.getpatientName() << endl;
        if (Pres)
            cout << "Prescription ID: " << Pres->getPrescriptionId() << endl;
        cout << "Total Amount: " << totalAmount << " EGP\n";
        cout << "Payment Method: " << paymentMethod << endl;
        cout << "=========================\n";
    }

    void processPayment() {
        if (!Pres) { cout << "No prescription found. Payment cancelled.\n"; return; }
        calculateTotal();
        displayBill();
        choosePaymentMethod();
        cout << "\nPayment Completed Successfully.\n";
    }
};
int payment::count = 0;

// ================== MAIN ==================
int main() {
    vector<doctor> doctors;
    vector<patient> patients;
    vector<prescription> prescriptions;

    // Example: Adding some doctors
    doctors.push_back(doctor("hossam", 12345, "doctor", "Hossam Anwar", "male", "Giza", 01124842257, 35, "General", 8));
    doctors.push_back(doctor("laila", 54321, "doctor", "Laila Omar", "female", "Cairo", 01012345678, 40, "Dentistry", 8));

    int choice;
    while (true) {
        cout << "\n=== Hospital System ===\n";
        cout << "1.Register Patient\n2.Patient Login\n3.Doctor Login\n4.View Doctors\n5.Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) { // Register Patient
            string n, g, ad, u, di; long long ph; int pass, ag;
            cout << "Username: "; cin >> u;
            bool exist = false;
            for (auto &p : patients)
                if (p.get_username() == u) { exist = true; break; }
            if (exist) { cout << "Username already exists.\n"; continue; }

            cout << "Password: "; cin >> pass;
            cout << "Name: "; cin >> n;
            cout << "Gender: "; cin >> g;
            cout << "Address: "; cin >> ad;
            cout << "Phone: "; cin >> ph;
            cout << "Age: "; cin >> ag;
            cout << "Disease: "; cin >> di;

            // Choose specialization
            cout << "Choose specialization:\n1. General\n2. Dentistry\n3. Dermatology\n4. Cardiology\nChoice: ";
            int spchoice; cin >> spchoice;
            string spec;
            if (spchoice==1) spec="General";
            else if (spchoice==2) spec="Dentistry";
            else if (spchoice==3) spec="Dermatology";
            else spec="Cardiology";

            // Show available doctors for that specialization
            vector<doctor*> available;
            cout << "Available doctors:\n";
            int idx=1;
            for (auto &d : doctors)
                if (d.getSpecialization() == spec) { cout << idx << ". " << d.getDoctorName() << endl; available.push_back(&d); idx++; }
            if (available.empty()) { cout << "No doctor available for this specialization.\n"; continue; }

            cout << "Choose doctor: ";
            int dchoice; cin >> dchoice;
            if (dchoice <1 || dchoice>available.size()) { cout << "Invalid choice.\n"; continue; }

            patient p(n, g, ad, ph, ag, u, pass, "patient", di, "", 0);
            patients.push_back(p);

            // Add patient to chosen doctor's queue
            available[dchoice-1]->addPatient(p.getPatientId(), p.getpatientName());
            cout << "Patient registered successfully!\n";
        }

        else if (choice == 2) { // Patient Login
            string u; int pass;
            cout << "Username: "; cin >> u;
            cout << "Password: "; cin >> pass;
            patient* curr = nullptr;
            for (auto &p : patients) if (p.get_username()==u && p.login(pass)) { curr=&p; break; }
            if (!curr) { cout << "Login failed.\n"; continue; }

            int sub;
            do {
                cout << "\n--- Patient Menu ---\n1.View Prescription\n2.Show Info\n3.View / Pay Bill\n4.Logout\nChoice: ";
                cin >> sub;

                if (sub==1) {
                    bool found=false;
                    for (auto &pres : prescriptions)
                        if (pres.getPatientId()==curr->getPatientId()) { pres.showPrescription(); found=true; }
                    if (!found) cout << "No prescription found.\n";
                }
                else if (sub==2) curr->display();
                else if (sub==3) {
                    prescription* presPtr=nullptr;
                    for (auto &pres : prescriptions)
                        if (pres.getPatientId()==curr->getPatientId()) { presPtr=&pres; break; }
                    if (!presPtr) { cout << "No prescription found.\n"; continue; }
                    payment pay(*curr, presPtr);
                    pay.processPayment();
                }

            } while (sub!=4);
        }

        else if (choice == 3) { // Doctor Login
            string u; int pass;
            cout << "Username: "; cin >> u;
            cout << "Password: "; cin >> pass;
            doctor* curr = nullptr;
            for (auto &d : doctors) if (d.getUsername()==u && d.login(pass)) { curr=&d; break; }
            if (!curr) { cout << "Login failed.\n"; continue; }

            int sub;
            do {
                cout << "\n--- Doctor Menu ---\n1.Add Prescription\n2.Serve Patient\n3.Show Next Patient\n4.Show Info\n5.Logout\nChoice: ";
                cin >> sub;

                if (sub==1) {
                    int pid;
                    cout << "Enter Patient ID to prescribe: "; cin >> pid;
                    patient* pat=nullptr;
                    for (auto &p : patients) if (p.getPatientId()==pid) { pat=&p; break; }
                    if (!pat) { cout << "Patient not found.\n"; continue; }

                    prescription pres(*curr, *pat);
                    string med; float price;
                    cout << "Enter medicines (type 'done' to finish):\n";
                    while(true){
                        cout << "Medicine name: "; cin >> med;
                        if(med=="done") break;
                        cout << "Price: "; cin >> price;
                        pres.addMedicine(med, price);
                    }
                    prescriptions.push_back(pres);
                    cout << "Prescription added successfully.\n";
                }

                else if (sub==2) curr->servePatient();
                else if (sub==3) curr->showNextPatient();
                else if (sub==4) curr->displayDoctor();

            } while(sub!=5);
        }

        else if (choice == 4) {
            for(auto &d : doctors) d.displayDoctor();
        }

        else if (choice == 5) break;

        else cout << "Invalid choice!\n";
    }

    return 0;
}
