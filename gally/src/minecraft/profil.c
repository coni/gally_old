int mc_IsUsernameCorrect(char* username)
{
    int correct = 1;
    for (int i = 0; username[i] != '\0'; i++)
    {
        if ((username[i] < 48 || (username[i] > 57 && username[i] < 65) || (username[i] > 90 && username[i] < 95) || username[i] == 96 || username[i] > 122))
                correct = 0;
    }
    return correct;
}
