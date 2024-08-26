#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "ansiEsc.h"
#include "input.h"

// #define NDEBUG
#include <assert.h>

#include "../namespaces.h"

using namespace std;
using namespace clients;
using namespace terminal;
using namespace commands;

// --- Function Prototypes
// void ignoreInput();
bool booleanQuestion(string message);
void wrongCommand(cmdStatus cmdStatus);
void wrongClientData(invalidClient wrongData);
void checkClientStatus(clientStatus clientStatus);
void pressEnterToCont(string message, bool warning);
string getLower(string word);
float getFloat(string message, float low, float high);

// --- Functions

/*
// This prevents the program to crash if there's more input
void ignoreInput()
{
  while (getchar() != '\n')
    cin.clear();
}
*/

// Boolean Question
bool booleanQuestion(string message)
{
  string input;
  char c;

  while (true)
  {
    cout << "- " << message << " [y/N] ";
    getline(cin, input);

    c = tolower(input[0]);
    if (c == 'y')
    {
      cout << '\n';
      return true;
    }
    else if (c == 'n')
    {
      cout << '\n';
      return false;
    }
    else
    {
      assert(c != 'y' && c != 'n'); // c Must be Different from Y or N
      wrongCommand(wrongBooleanAnswer);
    }
  }
}

// Function to Check if the Command Entered by the User is Correct
void wrongCommand(cmdStatus cmdStatus)
{
  string message = "ERROR: ";

  switch (cmdStatus)
  {
  case wrongBooleanAnswer:
    message.append("It's a Yes/No Question. You must type 'y', 'Y' or 'n', 'N'");
    break;
  case noCmd:
  case wrongMainCmd:
  case wrongSubCmd:
    if (cmdStatus == noCmd)
      message.append("No Command");
    else
      message.append((cmdStatus == wrongMainCmd) ? "Wrong Main Command" : "Wrong Subcommand");
    message.append(". Press ENTER to Display Help Message");
    break;
  case wrongViewClientsCmd:
  case wrongFilterClientsCmd:
  case wrongSortByParam:
  case wrongFieldParam:
  case wrongField:
    if (cmdStatus == wrongFilterClientsCmd || cmdStatus == wrongViewClientsCmd)
      message.append("Wrong Command");
    else if (cmdStatus == wrongSortByParam || cmdStatus == wrongFieldParam || cmdStatus == wrongField)
      message.append((cmdStatus == wrongSortByParam) ? "Wrong Sort By Parameter" : "Wrong Field");
    message.append(". Press ENTER to Display Search Data Parameters Message");
    break;
  }
  pressEnterToCont(message, true);
}

// Function to Check if the Client Data Typed by the User is Correct
void wrongClientData(invalidClient wrongData)
{
  string message = "Invalid: Client ";

  switch (wrongData)
  {
  case invalidClientId:
    message.append("ID");
    break;
  case invalidClientAccountNumber:
    message.append("Account Number");
    break;
  case clientExists:
    message = "Error: Client Exists";
    break;
  }
  pressEnterToCont(message, true);
}

// Function to Check if the Program Could Find a Client
void checkClientStatus(clientStatus clientStatus)
{
  string message;

  switch (clientStatus)
  {
  case clientNotFound:
    message = "Error 404: Client Not Found. Run \"Add Client Command\"";
    break;
  case clientSuspended:
    message = "Error: Your Account is Suspended. Contact Us to Get Access to All of your Account Features";
    break;
  }
  pressEnterToCont(message, (clientStatus != clientFound) ? true : false);
}

// Function to Stop the Program Flow while the User doesn't press the ENTER key
void pressEnterToCont(string message, bool warning)
{
  string _temp;

  printTitle(message, applyBgColor, applyFgColor, warning);
  getline(cin, _temp);
  // ignoreInput();
}

// Function to Ask for a Float Input
float getFloat(string message, float low, float high)
{
  string temp;
  float amount;

  while (true)
    try // Get Float
    {
      cout << message << ": ";
      getline(cin, temp);
      amount = stof(temp);

      // Check if the Number is Out of Range
      if (amount < low || amount > high)
        throw out_of_range("Amount Out of Range");

      return amount;
    }
    catch (const invalid_argument &e)
    {
      pressEnterToCont("ERROR: Invalid Argument. It Must be a Float", true);
    }
    catch (const out_of_range &e)
    {
      ostringstream stream;

      stream << "The Number is Out of that Range"
             << " [" << fixed << setprecision(precision) << low << '-' << high << "]";

      pressEnterToCont(stream.str(), true);
    }
}

// Function to Get a Lowercase String
string getLower(string word)
{
  string wordToLower;

  for (int i = 0; i < word.length(); i++)
    wordToLower += tolower(word[i]); // Append Character in Lowercase

  return wordToLower;
}