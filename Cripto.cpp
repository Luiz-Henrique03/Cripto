#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

const string password = "senha123"; // Senha padrão para criptografia e descriptografia

void encryptFile(const fs::path& path, const string& password)
{
    string filename = path.filename().string();
    cout << "Criptografando arquivo " << filename << "..." << endl;

    ifstream fin(path, ios::binary);
    if (!fin)
    {
        cerr << "Erro ao abrir o arquivo " << filename << endl;
        return;
    }

    string output_filename = filename + ".encrypted";
    ofstream fout(output_filename, ios::binary);
    if (!fout)
    {
        cerr << "Erro ao criar o arquivo " << output_filename << endl;
        fin.close();
        return;pr
    }

    char c;
    while (fin >> noskipws >> c)
    {
        int temp = c ^ password[0];
        fout << (char)temp;
    }

    fin.close();
    fout.close();

    fs::remove(path);
}

void decryptFile(const fs::path& path, const string& password)
{
    string filename = path.filename().string();
    if (!fs::path(filename).has_extension() || fs::path(filename).extension() != ".encrypted")
    {
        cerr << "O arquivo " << filename << " não é um arquivo criptografado." << endl;
        return;
    }

    filename = fs::path(filename).stem().string();
    cout << "Descriptografando arquivo " << filename << "..." << endl;

    ifstream fin(path, ios::binary);
    if (!fin)
    {
        cerr << "Erro ao abrir o arquivo " << filename << endl;
        return;
    }

    string output_filename = filename;
    ofstream fout(output_filename, ios::binary);
    if (!fout)
    {
        cerr << "Erro ao criar o arquivo " << output_filename << endl;
        fin.close();
        return;
    }

    char c;
    while (fin >> noskipws >> c)
    {
        int temp = c ^ password[0];
        fout << (char)temp;
    }

    fin.close();
    fout.close();

    fs::remove(path);
}

int main()
{
    int option;
    cout << "Digite 1 para criptografar arquivos ou 2 para descriptografar arquivos: ";
    cin >> option;

    if (option == 1)
    {
        for (const auto& entry : fs::directory_iterator("."))
        {
            if (fs::is_regular_file(entry.path()))
            {
                if (entry.path().filename().string() == "teste.exe" || entry.path().filename().string() == "Cripto.cpp")
                {
                    continue;
                }

                encryptFile(entry.path(), password);
            }
        }
    }

    else if (option == 2)
    {
        string password;
        cout << "Digite a senha de descriptografia: ";
        cin >> password;

        for (const auto& entry : fs::directory_iterator("."))
        {
            if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".encrypted")
            {
                string filename = entry.path().filename().string();
                cout << "Descriptografando arquivo " << filename << "..." << endl;

                ifstream fin(entry.path(), ios::binary);
                if (!fin)
                {
                    cerr << "Erro ao abrir o arquivo " << filename << endl;
                    continue;
                }

                string output_filename = filename.substr(0, filename.length() - 10); // remove a extensão ".encrypted" do nome do arquivo
                ofstream fout(output_filename, ios::binary);
                if (!fout)
                {
                    cerr << "Erro ao criar o arquivo " << output_filename << endl;
                    fin.close();
                    continue;
                }

                char c;
                while (fin >> noskipws >> c)
                {
                    int temp = c ^ password[0];
                    fout << (char)temp;
                }

                fin.close();
                fout.close();

                fs::remove(entry.path());
            }
        }

        cout << "Descriptografia concluida!" << endl;
    }
}
