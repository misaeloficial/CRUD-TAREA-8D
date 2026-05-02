#include "conexion.h"

// =============================================
// Definicion de las variables globales
// (solo se definen UNA vez, aqui)
// =============================================
SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;

// =============================================
// Funcion: conectar()
// Establece la conexion con PostgreSQL via ODBC.
// Retorna true si la conexion fue exitosa,
// false si hubo algun error.
// =============================================
bool conectar()
{
  // Paso 1: Crear el ambiente ODBC
  SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
  {
    cout << "[ERROR] No se pudo crear el ambiente ODBC." << endl;
    return false;
  }

  // Paso 2: Indicar que usamos ODBC version 3
  SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);

  // Paso 3: Crear el manejador de conexion
  ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
  {
    cout << "[ERROR] No se pudo crear el manejador de conexion." << endl;
    return false;
  }

  // Paso 4: Conectarse usando el DSN configurado en Windows
  // IMPORTANTE: Cambia los valores segun tu configuracion:
  //   - "universidad_dsn" -> nombre del DSN que creaste en ODBC
  //   - "postgres"        -> tu usuario de PostgreSQL
  //   - "tu_password"     -> tu contrasena de PostgreSQL
  ret = SQLConnect(
      hDbc,
      (SQLCHAR *)"universidad_dsn", SQL_NTS,
      (SQLCHAR *)"postgres", SQL_NTS,
      (SQLCHAR *)"Lahermanadejulio", SQL_NTS);

  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
  {
    cout << "[ERROR] No se pudo conectar a la base de datos." << endl;
    cout << "Verifica que el DSN este bien configurado." << endl;
    return false;
  }

  cout << "========================================" << endl;
  cout << "  Conexion exitosa a PostgreSQL!" << endl;
  cout << "========================================" << endl;
  return true;
}

// =============================================
// Funcion: cerrarConexion()
// Cierra la conexion de forma ordenada.
// Siempre debe llamarse antes de salir.
// =============================================
void cerrarConexion()
{
  SQLDisconnect(hDbc);
  SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
  SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
  cout << "\nConexion cerrada correctamente. Hasta luego!" << endl;
}