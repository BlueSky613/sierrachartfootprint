
import FootPrint from "../model/FootprintInfo.js";
//* POST /footprint
const postFootPrint = async (req, res) => {
    try {
        const json_str = req.body['Message'];
        const json_obj = JSON.parse(json_str);
        const { time, open, high, low, close, priceLevels } = json_obj;
        console.log(time);

        //Create a new FootPrint instance and save it to the database
        const newFootPrint = await FootPrint.create({
            time,
            open,
            high,
            low,
            close,
            priceLevels,
        });
        res.status(201).json(newFootPrint);
    } catch (error) {
        console.log("time");
        res.status(500).json({ msg: error.message });
    }
};

//* PUT /forgotpwd
const putFootPrint = async (req, res) => {
    try {
        const { email, password } = req.body;
        const userAccount = await UserAccount.findOne({ where: { email } });
        if (!userAccount) {
            return res.status(404).json({ msg: "User does not exist." });
        }
        const newPasswordHash = await bcrypt.hash(password, 10);
        userAccount.password = newPasswordHash;
        await userAccount.save();
        res.status(200).json({ msg: "Successfully reset password." });
    } catch (error) {
        res.status(500).json({ msg: error.message });
    }
};

export const controller = {
    postFootPrint,
    putFootPrint
};